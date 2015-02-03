#include "CalculateThread.h"


// RsaToolbox
#include "Touchstone.h"
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalculateThread::CalculateThread(QObject *parent) :
    QThread(parent)
{
    _data = NULL;
    reset();
}
CalculateThread::~CalculateThread()
{
    deleteChannels();
    if (isData() && _data->isVna()) {
        _data->vna()->clearStatus();
        _data->vna()->settings().errorDisplayOn();
    }
}

void CalculateThread::run() {
    if (!isReady() || !areFilesWritable()) {
        _isError = true;
        return;
    }

    reset();
    deleteFiles();

    loadCalibrations();
    _calibratedPorts = _outerData.ports();
    _portsLeft = *(_data->ports());
    calculateFrequency();

    getVnaPorts();
    while (isPortsLeft()) {
        bool isPort1Matrix;
        uint port1, vnaPort1;
        bool isPort2Matrix;
        uint port2, vnaPort2;
        if (!portPair(port1, vnaPort1, isPort1Matrix,
                      port2, vnaPort2, isPort2Matrix))
        {
            _isError = true;
            deleteChannels();
            deleteFiles();
            return;
        }
        calculate(port1, vnaPort1, isPort1Matrix,
                  port2, vnaPort2, isPort2Matrix);
    }
}

bool CalculateThread::isData() const {
    return _data != NULL;
}
Data *CalculateThread::data() const {
    return _data;
}
void CalculateThread::setData(Data *data) {
    _data = data;
}

bool CalculateThread::isReady() {
    reset();

    if (!isData())
        return false;

    if (!hasPorts()
            || !hasCalibrations())
        return false;

    if (!loadCalibrations())
        return false;

    if (!calibrationsMatch()
            || !moreThanOnePortCalibrated()
            || !portsAreCalibrated()
            || !frequencyIsKnown())
    {
        deleteChannels();
        return false;
    }

    deleteChannels();
    return true;
}

bool CalculateThread::isError() const {
    return _isError;
}

bool CalculateThread::areFilesWritable() {
    if (_data->filePathNames().isEmpty()) {
        emit error("*Choose filenames for results");
        return false;
    }

    QFile test(_data->filePathNames().first());
    if (!test.open(QFile::WriteOnly)) {
        emit error("*Directory is not writable");
        return false;
    }
    else {
        test.close();
        test.remove();
        return true;
    }
}
bool CalculateThread::hasPorts() {
    if (_data->ports()->isEmpty()) {
        emit error("*Choose ports");
        return false;
    }

    return true;
}
bool CalculateThread::hasCalibrations() {
    if (_data->outerCalibration()->isEmpty()) {
        emit error("*Choose outer calibration");
        return false;
    }
    if (_data->innerCalibration()->isEmpty()) {
        emit error("*Choose inner calibration");
        return false;
    }

    return true;
}
bool CalculateThread::loadCalibrations() {
    _deleteOuterChannel = false;
    _deleteInnerChannel = false;

    if (!_data->outerCalibration()->canLoad(_data->vna())) {
        emit error("*Cannot load outer calibration");
        return false;
    }
    if (!_data->innerCalibration()->canLoad(_data->vna())) {
        emit error("*Cannot load inner calibration");
        return false;
    }

    if (_data->outerCalibration()->isChannel()) {
        _outerChannel = _data->outerCalibration()->channel();
    }
    else {
        _deleteOuterChannel = true;
        _outerChannel = _data->vna()->createChannel();
        _data->vna()->channel(_outerChannel).setCalGroup(_data->outerCalibration()->calGroup());
    }
    _outerData = _data->vna()->channel(_outerChannel).corrections();

    if (_data->innerCalibration()->isChannel()) {
        _innerChannel = _data->innerCalibration()->channel();
    }
    else {
        _deleteInnerChannel = true;
        _innerChannel = _data->vna()->createChannel();
        _data->vna()->channel(_innerChannel).setCalGroup(_data->innerCalibration()->calGroup());
    }
    _innerData = _data->vna()->channel(_innerChannel).corrections();

    return true;
}
bool CalculateThread::calibrationsMatch() {
    // Cal type, ports
    if (_outerData.calibrationType() != _innerData.calibrationType()) {
        emit error("*Calibration methods do not match");
        return false;
    }
    if (_outerData.ports() != _innerData.ports()) {
        emit error("*Calibration ports do not match");
        return false;
    }


    // Sweep
    if (_outerData.sweepType() != _innerData.sweepType()) {
        emit error("*Sweep types do not match");
        return false;
    }
    if (_outerData.startFrequency_Hz() != _innerData.startFrequency_Hz()
            || _outerData.stopFrequency_Hz() != _innerData.stopFrequency_Hz())
    {
        emit error("*Frequencies do not match");
        return false;
    }
    if (_outerData.points() != _innerData.points()) {
        emit error("*Points do not match");
        return false;
    }

    // Hardware
    uint numMats = _outerData.switchMatrices();
    if (_outerData.testPortToVnaMap() != _innerData.testPortToVnaMap()
            || numMats != _innerData.switchMatrices())
    {
        emit error("*Hardware configurations do not match");
        return false;
    }
    if (numMats == 0)
        return true;


    QVector<uint> switchMatrices = range(uint(1), numMats);
    foreach (uint i, switchMatrices)
    {
        if (_outerData.switchMatrixToVnaPortMap(i) != _innerData.switchMatrixToVnaPortMap(i)
                || _outerData.testPortToSwitchMatrixMap(i) != _innerData.testPortToSwitchMatrixMap(i))
        {
            emit error ("*Hardware configurations do not match");
            return false;
        }
    }

    return true;
}
bool CalculateThread::moreThanOnePortCalibrated() {
    if (_outerData.ports().size() <= 1) {
        emit error("*Must calibrate more than one port");
        return false;
    }

    return true;
}

bool CalculateThread::portsAreCalibrated() {
    QVector<uint> calibratedPorts = _outerData.ports();
    QVector<uint> uncalibratedPorts;
    foreach (uint i, *(_data->ports())) {
        if (!calibratedPorts.contains(i))
            uncalibratedPorts << i;
    }

    if (uncalibratedPorts.isEmpty()) {
        return true;
    }
    else {
        QString msg;
        if (uncalibratedPorts.size() == 1) {
            msg = "*Port %1 is not calibrated";
            msg = msg.arg(uncalibratedPorts.first());
        }
        else {
            msg = "*Ports %1 are not calibrated";
            msg = msg.arg(portString(uncalibratedPorts));
        }
        emit error(msg);
        return false;
    }
}
bool CalculateThread::frequencyIsKnown() {
    if (_outerData.sweepType() == VnaChannel::SweepType::Segmented) {
        emit error("*Segmented sweep is not supported");
        return false;
    }

    return true;
}
void CalculateThread::calculateFrequency() {
    const VnaChannel::SweepType sweepType = _outerData.sweepType();
    const double start = _outerData.startFrequency_Hz();
    const double stop = _outerData.stopFrequency_Hz();
    const int points = _outerData.points();

    if (sweepType == VnaChannel::SweepType::Linear)
        _frequency_Hz = linearSpacing(start, stop, points);
    else
        _frequency_Hz = logSpacing(start, stop, points);
}

bool CalculateThread::isPortsLeft() const {
    return !_portsLeft.isEmpty();
}
void CalculateThread::getVnaPorts() {
    _vnaPorts = _outerData.testPortToVnaMap().values().toVector();
    uint switchMatrices = _outerData.switchMatrices();
    for (uint i = 1; i <= switchMatrices; i++)
        _vnaPorts += _outerData.switchMatrixToVnaPortMap(i).values().toVector();
    qSort(_vnaPorts);
}
bool CalculateThread::portPair(uint &port1, uint &vnaPort1, bool &isPort1Matrix, uint &port2, uint &vnaPort2, bool &isPort2Matrix) {
    _data->vna()->printLine("CalculateThread::portPair(...)\n");
    _data->vna()->clearStatus();
    _data->vna()->settings().errorDisplayOff();
    QVector<uint> availableVnaPorts = _vnaPorts;

    port1 = 0;
    vnaPort1 = 0;
    isPort1Matrix = false;
    port2 = 0;
    vnaPort2 = 0;
    isPort2Matrix = false;

    // port1, vnaPort1
    port1 = _portsLeft.takeFirst();
    if (_outerData.isVnaPort(port1)) {
        vnaPort1 = _outerData.vnaPort(port1);
        availableVnaPorts.remove(availableVnaPorts.indexOf(vnaPort1));
    }
    else {
        foreach (uint vnaPort, availableVnaPorts) {
            _data->vna()->clearStatus();
            if (_outerData.directivity(port1, vnaPort, port1, vnaPort).size() != 0
                    && !_data->vna()->isError())
            {
                vnaPort1 = vnaPort;
                availableVnaPorts.remove(availableVnaPorts.indexOf(vnaPort));
                isPort1Matrix = true;
                break;
            }
        }
        _data->vna()->clearStatus();
    }
    if (vnaPort1 == 0) {
        _data->vna()->clearStatus();
        _data->vna()->settings().errorDisplayOn();
        QString message = "*Error extracting port %1";
        message = message.arg(port1);
        emit error(message);
        return false;
    }

    foreach (uint port, _portsLeft) {
        if (_outerData.isVnaPort(port))
        {
            port2 = port;
            _portsLeft.remove(_portsLeft.indexOf(port2));
            vnaPort2 = _outerData.vnaPort(port2);
            break;
        }
        else {
            foreach (uint vnaPort, availableVnaPorts) {
                _data->vna()->clearStatus();
                if (_outerData.directivity(port, vnaPort, port, vnaPort).size() != 0
                        && !_data->vna()->isError())
                {
                    port2 = port;
                    _portsLeft.remove(_portsLeft.indexOf(port2));
                    vnaPort2 = vnaPort;
                    isPort2Matrix = true;
                    break;
                }
            }
        }
        if (port2 != 0)
            break;
    }
    if (port2 != 0) {
        _data->vna()->clearStatus();
        _data->vna()->settings().errorDisplayOn();
        return true;
    }

    // Search through remaining ports
    // remainder:
    // _calibratedPorts - _portsLeft
    QVector<uint> remainder = _calibratedPorts;
    remainder.remove(remainder.indexOf(port1));
    foreach (uint i, _portsLeft) {
        remainder.remove(remainder.indexOf(i));
    }

    foreach (uint port, remainder) {
        if (_outerData.isVnaPort(port))
        {
            port2 = port;
            _portsLeft.remove(_portsLeft.indexOf(port2));
            vnaPort2 = _outerData.vnaPort(port2);
            break;
        }
        else {
            foreach (uint vnaPort, availableVnaPorts) {
                _data->vna()->clearStatus();
                if (_outerData.directivity(port, vnaPort, port, vnaPort).size() != 0
                        && !_data->vna()->isError())
                {
                    port2 = port;
                    vnaPort2 = vnaPort;
                    isPort2Matrix = true;
                    break;
                }
            }
            if (port2 != 0)
                break;
        }
    }

    _data->vna()->clearStatus();
    _data->vna()->settings().errorDisplayOn();

    if (port2 != 0) {
        return true;
    }
    else {
        QString message = "*Error extracting port %1";
        message = message.arg(port1);
        emit error(message);
        return false;
    }
}
void CalculateThread::calculate(uint port1, uint vnaPort1, bool isPort1Matrix, uint port2, uint vnaPort2, bool isPort2Matrix)
{
    if (_data->ports()->contains(port1)) {
        const uint index = _data->ports()->indexOf(port1);
         const QString pathName = _data->filePathNames()[index];
        QFileInfo file(pathName);
        if (!file.exists()) {
            NetworkData s2p = calculateNetwork(port1, vnaPort1, isPort1Matrix,
                                               port2, vnaPort2, isPort2Matrix);
            Touchstone::write(s2p, pathName);
        }
    }
    if (_data->ports()->contains(port2)) {
        const uint index = _data->ports()->indexOf(port2);
        const QString pathName = _data->filePathNames()[index];
        QFileInfo file(pathName);
        if (!file.exists()) {
            NetworkData s2p = calculateNetwork(port2, vnaPort2, isPort2Matrix,
                                               port1, vnaPort1, isPort1Matrix);
            Touchstone::write(s2p, pathName);
        }
    }
}
NetworkData CalculateThread::calculateNetwork(uint port1, uint vnaPort1, bool isPort1Matrix, uint port2, uint vnaPort2, bool isPort2Matrix) {

    ComplexRowVector dir, reflTrack, srcMatch;
    if (isPort1Matrix || isPort2Matrix) {
        dir = _outerData.directivity(port2, vnaPort2, port1, vnaPort1);
        reflTrack = _outerData.reflectionTracking(port2, vnaPort2, port1, vnaPort1);
        srcMatch = _outerData.sourceMatch(port2, vnaPort2, port1, vnaPort1);
    }
    else {
        dir = _outerData.directivity(port2, port1);
        reflTrack = _outerData.reflectionTracking(port2, port1);
        srcMatch = _outerData.sourceMatch(port2, port1);
    }

    ComplexRowVector dirWithDut, reflTrackWithDut, srcMatchWithDut;
    if (isPort1Matrix || isPort2Matrix) {
        dirWithDut = _innerData.directivity(port2, vnaPort2, port1, vnaPort1);
        reflTrackWithDut = _innerData.reflectionTracking(port2, vnaPort2, port1, vnaPort1);
        srcMatchWithDut = _innerData.sourceMatch(port2, vnaPort2, port1, vnaPort1);
    }
    else {
        dirWithDut = _innerData.directivity(port2, port1);
        reflTrackWithDut = _innerData.reflectionTracking(port2, port1);
        srcMatchWithDut = _innerData.sourceMatch(port2, port1);
    }

    ComplexRowVector denominator = add(reflTrack, multiplyEach(srcMatch, subtract(dirWithDut, dir)));
    ComplexRowVector s11 = divideEach(subtract(dirWithDut, dir), denominator);
    ComplexRowVector s22 = subtract(srcMatchWithDut, divideEach(multiplyEach(srcMatch, reflTrackWithDut), denominator));

    ComplexRowVector s21Numerator = smoothSqrt(multiplyEach(reflTrack, reflTrackWithDut));
    ComplexRowVector s21 = divideEach(s21Numerator, denominator);
    s21 = smoothPhase(s21);
    s21 = fixPhaseAtDc(_frequency_Hz, s21);

    ComplexMatrix3D y;
    constructMatrix(y, s11, s21, s22);

    NetworkData data;
    data.setComment("");
    data.setParameter(NetworkParameter::S);
    data.setXUnits(Units::Hertz);
    data.setData(_frequency_Hz, y);
    return(data);
}
void CalculateThread::constructMatrix(ComplexMatrix3D &matrix, const ComplexRowVector &s11, const ComplexRowVector &s21, const ComplexRowVector &s22) {
    uint points = s11.size();
    matrix.resize(points);
        for (uint i = 0; i < points; i++) {
            matrix[i].resize(2);
            matrix[i][0].resize(2);
            matrix[i][1].resize(2);
            matrix[i][0][0] = s11[i];
            matrix[i][0][1] = s21[i];
            matrix[i][1][0] = s21[i];
            matrix[i][1][1] = s22[i];
        }
}

void CalculateThread::deleteChannels() {
    if (!isData() || !_data->isVna())
        return;

    if (_deleteOuterChannel && _data->vna()->isChannel(_outerChannel))
        _data->vna()->deleteChannel(_outerChannel);
    if (_deleteInnerChannel && _data->vna()->isChannel(_innerChannel))
        _data->vna()->deleteChannel(_innerChannel);

    _deleteOuterChannel = false;
    _deleteInnerChannel = false;
}
void CalculateThread::deleteFiles() {
    foreach (QString file, _data->filePathNames()) {
        QFileInfo info(file);
        if (info.exists()) {
            info.dir().remove(info.fileName());
        }
    }
}
void CalculateThread::reset() {
    _isError = false;
    _vnaPorts.clear();
    _calibratedPorts.clear();
    _portsLeft.clear();
    _deleteOuterChannel = false;
    _outerChannel = 0;
    _deleteInnerChannel = false;
    _innerChannel = 0;
    _frequency_Hz.clear();

}

#include "CalculateThread.h"


// RsaToolbox
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
{}

void CalculateThread::run() {
    if (!isReady()) {
        _isError = true;
        return;
    }

    reset();
    loadCalibrations();
    _calibratedPorts = _outerData.ports();
    _portsLeft = *(_data->ports());
    calculateFrequency();

    getVnaPorts();
    while (isPortsLeft()) {
        uint port1, vnaPort1;
        uint port2, vnaPort2;
        if (!portPair(port1, vnaPort1, port2, vnaPort2)) {
            _isError = true;
            deleteChannels();
            deleteFiles();
            return;
        }
        calculate(port1, vnaPort1, port2, vnaPort2);
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

    if (!isData()) {
        return false;
    }

    if (!hasPorts() || !hasCalibrations())
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

bool CalculateThread::hasPorts() {
    qDebug() << "hasPorts";
    qDebug() << *(_data->ports());
    if (_data->ports()->isEmpty()) {
        emit error("*Choose ports");
        return false;
    }

    return true;
}
bool CalculateThread::hasCalibrations() {
    qDebug() << "hasCalibrations";
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
    qDebug() << "loadCalibrations";
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
    qDebug() << "calibrationsMatch";
    // Cal type, ports
    qDebug() << "...type";
    if (_outerData.calibrationType() != _innerData.calibrationType()) {
        emit error("*Calibration methods do not match");
        return false;
    }
    qDebug() << "...ports";
    if (_outerData.ports() != _innerData.ports()) {
        emit error("*Calibration ports do not match");
        return false;
    }


    // Sweep
    qDebug() << "...sweep";
    if (_outerData.sweepType() != _innerData.sweepType()) {
        emit error("*Sweep types do not match");
        return false;
    }
    qDebug() << "...start, stop";
    if (_outerData.startFrequency_Hz() != _innerData.startFrequency_Hz()
            || _outerData.stopFrequency_Hz() != _innerData.stopFrequency_Hz())
    {
        emit error("*Frequencies do not match");
        return false;
    }
    qDebug() << "...points";
    if (_outerData.points() != _innerData.points()) {
        emit error("*Points do not match");
        return false;
    }

    // Hardware
    qDebug() << "...vna port map, # switch matrices";
    uint numMats = _outerData.switchMatrices();
    if (_outerData.testPortToVnaMap() != _innerData.testPortToVnaMap()
            || numMats != _innerData.switchMatrices())
    {
        emit error("*Hardware configurations do not match");
        return false;
    }
    if (numMats == 0)
        return true;


    qDebug() << "...each switch matrix";
    qDebug() << "count: " << _outerData.switchMatrices();
    QVector<uint> switchMatrices = range(uint(1), numMats);
    qDebug() << switchMatrices;
    foreach (uint i, switchMatrices)
    {
        qDebug() << "Switch matrix: " << i;
        if (_outerData.switchMatrixToVnaPortMap(i) != _innerData.switchMatrixToVnaPortMap(i)
                || _outerData.testPortToSwitchMatrixMap(i) != _innerData.testPortToSwitchMatrixMap(i))
        {
            emit error ("*Hardware configurations do not match");
            return false;
        }
    }

    qDebug() << "survived!";
    return true;
}
bool CalculateThread::moreThanOnePortCalibrated() {
    qDebug() << "moreThanOnePortCalibrated";
    if (_outerData.ports().size() <= 1) {
        emit error("*Must calibrate more than one port");
        return false;
    }

    return true;
}

bool CalculateThread::portsAreCalibrated() {
    qDebug() << "portsAreCalibrated";
    QVector<uint> calibratedPorts = _outerData.ports();
    foreach (uint i, *(_data->ports())) {
        if (!calibratedPorts.contains(i)) {
            emit error("*Ports are not calibrated");
            return false;
        }
    }

    return true;
}
bool CalculateThread::frequencyIsKnown() {
    qDebug() << "frequencyIsKnown";
    if (_outerData.sweepType() == VnaChannel::SweepType::Segmented) {
        emit error("*Segmented sweep is not supported");
        return false;
    }

    return true;
}
void CalculateThread::calculateFrequency() {
    qDebug() << "calculateFrequency";
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
    for (uint i = 0; i < switchMatrices; i++)
        _vnaPorts += _outerData.switchMatrixToVnaPortMap(i).values().toVector();
    qSort(_vnaPorts);
    qDebug() << "vna ports: " << _vnaPorts;
}
bool CalculateThread::portPair(uint &port1, uint &vnaPort1, uint &port2, uint &vnaPort2) {
    qDebug() << "CalculateThread::portPair";
    _data->vna()->settings().errorDisplayOff();
    QVector<uint> availableVnaPorts = _vnaPorts;

    // port1, vnaPort1
    qDebug() << "Ports left: " << _portsLeft;
    port1 = _portsLeft.takeFirst();
    qDebug() << "Taking " << port1;
    qDebug() << "Ports left: " << _portsLeft;
    qDebug() << "port1: " << port1;
    vnaPort1 = 0;
    if (_outerData.isVnaPort(port1)) {
        vnaPort1 = _outerData.vnaPort(port1);
        qDebug() << "vnaPort1: " << vnaPort1;
        availableVnaPorts.remove(availableVnaPorts.indexOf(vnaPort1));
    }
    else {
        qDebug() << "available vna ports: " << availableVnaPorts;
        foreach (uint vnaPort, availableVnaPorts) {
            qDebug() << "Trying vnaPort1="<<vnaPort;
            _data->vna()->clearStatus();
            if (_outerData.directivity(port1, vnaPort, port1, vnaPort).size() != 0
                    && !_data->vna()->isError())
            {
                vnaPort1 = vnaPort;
                availableVnaPorts.remove(availableVnaPorts.indexOf(vnaPort));
                qDebug() << "vnaPort1: " << vnaPort1;
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

    port2 = 0;
    vnaPort2 = 0;
    qDebug() << "ports left: " << _portsLeft;
    foreach (uint port, _portsLeft) {
        qDebug() << "Trying port2=" << port;
        if (_outerData.isVnaPort(port))
        {
            port2 = port;
            _portsLeft.remove(_portsLeft.indexOf(port2));
            vnaPort2 = _outerData.vnaPort(port2);
            break;
        }
        else {
            foreach (uint vnaPort, availableVnaPorts) {
                qDebug() << "Trying vnaPort2=" << vnaPort;
                _data->vna()->clearStatus();
                if (_outerData.directivity(port, vnaPort, port, vnaPort).size() != 0
                        && !_data->vna()->isError())
                {
                    port2 = port;
                    _portsLeft.remove(_portsLeft.indexOf(port2));
                    vnaPort2 = vnaPort;
                    break;
                }
            }
        }
        if (port2 != 0)
            break;
    }
    if (port2 != 0) {
        qDebug() << "port2: " << port2;
        qDebug() << "vnaPort2: " << vnaPort2;
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

    qDebug() << "Remaining ports: " << remainder;
    foreach (uint port, remainder) {
        qDebug() << "Trying port2=" << port;
        if (_outerData.isVnaPort(port))
        {
            port2 = port;
            _portsLeft.remove(_portsLeft.indexOf(port2));
            vnaPort2 = _outerData.vnaPort(port2);
            break;
        }
        else {
            foreach (uint vnaPort, availableVnaPorts) {
                qDebug() << "Trying vnaPort2=" << vnaPort;
                _data->vna()->clearStatus();
                if (_outerData.directivity(port, vnaPort, port, vnaPort).size() != 0
                        && !_data->vna()->isError())
                {
                    port2 = port;
                    vnaPort2 = vnaPort;
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
        qDebug() << "port2: " << port2;
        qDebug() << "vnaPort2: " << vnaPort2;
        return true;
    }
    else {
        QString message = "*Error extracting port %1";
        message = message.arg(port1);
        emit error(message);
        return false;
    }
}
void CalculateThread::calculate(uint port1, uint vnaPort1, uint port2, uint vnaPort2) {
    qDebug() << "Calculating " << port1 << vnaPort1 << port2 << vnaPort2;
}

void CalculateThread::deleteChannels() {
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

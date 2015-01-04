#include "Calibration.h"


//RsaToolbox
using namespace RsaToolbox;


SharedCalibration newCalibration() {
    return SharedCalibration(new Calibration());
}
SharedCalibration newCalibration(Vna *vna) {
    return SharedCalibration(new Calibration(vna));
}

Calibration::Calibration(QObject *parent) :
    QObject(parent)
{
    _vna = NULL;
    clear();
}
Calibration::Calibration(RsaToolbox::Vna *vna, QObject *parent) :
    _vna(vna),
    QObject(parent)
{
    clear();
}
Calibration::Calibration(const Calibration &other) :
    QObject()
{
    _vna = other._vna;
    _isCalGroup = other._isCalGroup;
    _calGroup = other._calGroup;
    _channel = other._channel;
}

Calibration::~Calibration()
{ }

bool Calibration::isEmpty() const {
    if (_isCalGroup && _calGroup.isEmpty())
        return true;
    if (!_isCalGroup && _channel == 0)
        return true;

    // Else
    return false;
}
bool Calibration::isCalGroup() const {
    if (!isEmpty() && _isCalGroup)
        return true;
    else
        return false;
}
bool Calibration::isChannel() const {
    if (!isEmpty() && !_isCalGroup)
        return true;
    else
        return false;
}

QString Calibration::calGroup() const {
    if (isCalGroup())
        return _calGroup;
    else
        return QString();
}
uint Calibration::channel() const {
    if (isChannel())
        return _channel;
    else
        return 0;
}

QString Calibration::displayText() const {
    if (isCalGroup()) {
        return calGroup();
    }
    if (isChannel()) {
        QString text = "Channel %1";
        text = text.arg(channel());
        return text;
    }

    // Else
    return QString();
}

void Calibration::operator=(const Calibration &other) {
    _vna = other._vna;
    _isCalGroup = other._isCalGroup;
    _calGroup = other._calGroup;
    _channel = other._channel;
    emit changed(displayText());
}

void Calibration::clear() {
    if (isEmpty())
        return;

    _isCalGroup = true;
    _calGroup.clear();
    _channel = 0;
    emit changed(displayText());
}
bool Calibration::setCalGroup(QString group) {
    if (isCalGroup() && _calGroup == group)
        return true;

    if (isVna()) {
        _vna->isError();
        _vna->clearStatus();
        uint c = _vna->createChannel();
        _vna->channel(c).setCalGroup(group);
        _vna->deleteChannel(c);
        if (_vna->isError()) {
            _vna->clearStatus();
            QString message = "Could not load cal group\n";
            message += "Make sure cal group is compatible with the current VNA setup";
            emit error(message);
            return false;
        }
    }

    _isCalGroup = true;
    _calGroup = group;
    _channel = 0;
    emit changed(displayText());
    return true;
}
bool Calibration::setChannel(uint index) {
    if (isChannel() && _channel == index)
        return true;

    if (isVna()) {
        if (!_vna->isChannel(index)) {
            QString message = "Channel %1 does not exist.";
            message = message.arg(index);
            emit error(message);
            return false;
        }
        if (!_vna->channel(index).isCalibrated()) {
            QString message = "Channel %1 is not calibrated.";
            message = message.arg(index);
            emit error(message);
            return false;
        }
    }

    _isCalGroup = false;
    _calGroup.clear();
    _channel = index;
    emit changed(displayText());
    return true;
}

bool Calibration::isVna() const {
    return _vna != NULL;
}

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
    clear();
}
Calibration::Calibration(const Calibration &other) :
    QObject()
{
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

bool Calibration::canLoad(Vna *vna) {
    if (vna == NULL)
        return false;
    if (isEmpty()) {
        emit error("*No calibration");
        return false;
    }
    if (isCalGroup()) {
        if (!vna->calGroups().contains(_calGroup)) {
            QString message = "*Cal group %1 doesn\'t exist";
            message = message.arg(_calGroup);
            emit error(message);
            return false;
        }
        else {
            vna->isError();
            vna->clearStatus();
            vna->settings().errorDisplayOff();
            uint c = vna->createChannel();
            vna->channel(c).setCalGroup(_calGroup);
            if (vna->isError()) {
                vna->deleteChannel(c);
                vna->clearStatus();
                vna->settings().errorDisplayOn();
                emit error("*Could not load cal group\n");
                return false;
            }
            else {
                vna->deleteChannel(c);
                vna->clearStatus();
                vna->settings().errorDisplayOn();
            }
        }
    }
    else {
        if (!vna->isChannel(_channel)) {
            QString message = "*Channel %1 does not exist.";
            message = message.arg(_channel);
            emit error(message);
            return false;
        }
        if (!vna->channel(_channel).isCalibrated()) {
            QString message = "*Channel %1 is not calibrated.";
            message = message.arg(_channel);
            emit error(message);
            return false;
        }
    }

    // Else
    return true;
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

    _isCalGroup = true;
    _calGroup = group;
    _channel = 0;
    emit changed(displayText());
    return true;
}
bool Calibration::setChannel(uint index) {
    if (isChannel() && _channel == index)
        return true;

    _isCalGroup = false;
    _calGroup.clear();
    _channel = index;
    emit changed(displayText());
    return true;
}

bool operator==(const Calibration &rhs, const Calibration &lhs) {
    if (rhs.isEmpty() != lhs.isEmpty())
        return false;
    if (rhs.isEmpty() && lhs.isEmpty())
        return true;
    if (rhs.isCalGroup() != lhs.isCalGroup())
        return false;
    if (rhs.isCalGroup())
        return rhs.calGroup() == lhs.calGroup();
    else
        return rhs.channel() == lhs.channel();
}
bool operator!=(const Calibration &rhs, const Calibration &lhs) {
    return !(rhs == lhs);
}

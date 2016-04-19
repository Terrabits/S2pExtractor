#include "Calibration.h"


//RsaToolbox
using namespace RsaToolbox;


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
Calibration::~Calibration() {
    //
}

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

Channel Calibration::load(Vna *vna) const {
    if (isChannel())
        return Channel(_channel, vna);
    else
        return Channel(_calGroup, vna);
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

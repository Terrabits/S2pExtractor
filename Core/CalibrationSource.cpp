#include "CalibrationSource.h"


CalibrationSource::CalibrationSource()
{
    clear();
}
CalibrationSource::CalibrationSource(uint channel)
{
    clear();
    setChannel(channel);
}
CalibrationSource::CalibrationSource(QString calGroup)
{
    clear();
    setCalGroup(calGroup);
}

CalibrationSource::~CalibrationSource()
{
    //
}

bool CalibrationSource::isEmpty() const {
    return !_channel && _calGroup.isEmpty();
}

bool CalibrationSource::isChannel() const {
    return _channel;
}
uint CalibrationSource::channel() const {
    return _channel;
}
void CalibrationSource::setChannel(uint index) {
    clear();
    _channel = index;
}

bool CalibrationSource::isCalGroup() const {
    return !_calGroup.isEmpty();
}
QString CalibrationSource::calGroup() const {
    return _calGroup;
}
void CalibrationSource::setCalGroup(QString calGroup) {
    clear();
    if (calGroup.endsWith(".cal", Qt::CaseInsensitive))
        calGroup.chop(4);
    _calGroup = calGroup;
}

void CalibrationSource::clear() {
    _channel = 0;
    _calGroup.clear();
}

QString CalibrationSource::displayText() const {
    if (isCalGroup()) {
        return calGroup();
    }
    if (isChannel()) {
        return QString("Channel %1").arg(channel());
    }

    return QString();
}

bool operator==(const CalibrationSource &left, const CalibrationSource &right) {
    if (left.isEmpty() && right.isEmpty()) {
        return true;
    }
    if (left.isChannel() && right.isChannel()) {
        return left.channel() == right.channel();
    }
    if (left.isCalGroup() && right.isCalGroup()) {
        return (left.calGroup().compare(right.calGroup(), Qt::CaseInsensitive) == 0);
    }

    // different source types
    return false;
}
bool operator!=(const CalibrationSource &left, const CalibrationSource &right) {
    return !(left == right);
}

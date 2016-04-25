#include "CalibrationSource.h"


CalibrationSource::CalibrationSource() :
    _channel(0)
{
    //
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
void CalibrationSource::setChannel(uint index) {
    clear();
    _channel = index;
}

bool CalibrationSource::isCalGroup() const {
    return !_calGroup.isEmpty();
}
void CalibrationSource::setCalGroup(QString calGroup) {
    clear();
    _calGroup = calGroup;
}

void CalibrationSource::clear() {
    _channel = 0;
    _calGroup.clear();
}

#include "progress.h"
#include <General.h>

Progress::Progress(uint majorEvents, uint minorEvents) :
    _iMinor(0),
    _iMajor(0),
    _majorEvents(majorEvents),
    _minorEvents(minorEvents)
{

}

void Progress::majorIncrement() {
    _iMinor = 0;
    if (_iMajor == _majorEvents) {
        return;
    }

    _iMajor++;
}
void Progress::minorIncrement() {
    if (_iMinor == _minorEvents) {
        return;
    }

    _iMinor++;
}

uint Progress::percent() const {
    const double perMajorIncrement = 1.0 / _majorEvents;
    const double perMinorIncrement = perMajorIncrement / (_minorEvents + 1);
    const double minorPercent = _iMinor * perMinorIncrement;
    const double majorPercent = _iMajor * perMajorIncrement;
    return RsaToolbox::round(100.0 * (majorPercent + minorPercent));
}

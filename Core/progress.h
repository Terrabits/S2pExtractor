#ifndef PROGRESS_H
#define PROGRESS_H

#include <Qt>

class Progress
{
public:
    Progress(uint majorEvents, uint minorEvents=3);

    void majorIncrement();
    void minorIncrement();
    uint percent() const;

private:
    uint _iMinor;
    uint _iMajor;
    const uint _majorEvents;
    const uint _minorEvents;
};

#endif // PROGRESS_H

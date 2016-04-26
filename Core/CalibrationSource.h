#ifndef CALIBRATION_SOURCE_H
#define CALIBRATION_SOURCE_H


// Qt
#include <QString>


class CalibrationSource
{
public:
    CalibrationSource();
    ~CalibrationSource();

    bool isEmpty() const;

    bool isChannel() const;
    void setChannel(uint index);

    bool isCalGroup() const;
    void setCalGroup(QString calGroup);

    void clear();

private:
    uint _channel;
    QString _calGroup;
};

#endif // CALIBRATION_SOURCE_H

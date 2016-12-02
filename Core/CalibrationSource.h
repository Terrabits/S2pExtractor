#ifndef CALIBRATION_SOURCE_H
#define CALIBRATION_SOURCE_H


// Qt
#include <QMetaType>
#include <QString>


class CalibrationSource
{
public:
    CalibrationSource();
    CalibrationSource(uint channel);
    CalibrationSource(QString calGroup);
    ~CalibrationSource();

    bool isEmpty() const;

    bool isChannel() const;
    uint channel() const;
    void setChannel(uint index);

    bool isCalGroup() const;
    QString calGroup() const;
    void setCalGroup(QString calGroup);

    void clear();

    QString displayText() const;

private:
    uint _channel;
    QString _calGroup;
};

Q_DECLARE_METATYPE(CalibrationSource)

#endif // CALIBRATION_SOURCE_H

#ifndef CALIBRATION_SOURCE_H
#define CALIBRATION_SOURCE_H


// Qt
#include <QDataStream>
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
    void clear();

    bool isChannel() const;
    uint channel() const;
    void setChannel(uint index);

    bool isCalGroup() const;
    QString calGroup() const;
    void setCalGroup(QString calGroup);

    QString displayText() const;

    void read (QDataStream &stream);
    void write(QDataStream &stream) const;

private:
    uint _channel;
    QString _calGroup;
};

QDataStream &operator<<(QDataStream &stream, const CalibrationSource &source);
QDataStream &operator>>(QDataStream &stream, CalibrationSource &source);

bool operator==(const CalibrationSource &left, const CalibrationSource &right);
bool operator!=(const CalibrationSource &left, const CalibrationSource &right);
Q_DECLARE_METATYPE(CalibrationSource)

#endif // CALIBRATION_SOURCE_H

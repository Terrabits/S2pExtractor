#ifndef CALIBRATION_H
#define CALIBRATION_H


//RsaToolbox
#include "Vna.h"

// Qt
#include <QObject>
#include <QSharedPointer>


class Calibration;
typedef QSharedPointer<Calibration> SharedCalibration;
SharedCalibration newCalibration();
SharedCalibration newCalibration(RsaToolbox::Vna *vna);

class Calibration : public QObject
{
    Q_OBJECT
public:
    explicit Calibration(QObject *parent = 0);
    Calibration(RsaToolbox::Vna *vna, QObject *parent = 0);
    Calibration(const Calibration &other);
    ~Calibration();

    bool isEmpty() const;
    bool isCalGroup() const;
    bool isChannel() const;

    QString calGroup() const;
    uint channel() const;

    QString displayText() const;

    void operator=(const Calibration &other);

signals:
    void error(QString message);
    void changed(QString displayText);

public slots:
    void clear();
    bool setCalGroup(QString group);
    bool setChannel(uint index);

private:
    bool isVna() const;
    RsaToolbox::Vna *_vna;

    bool _isCalGroup;
    QString _calGroup;
    uint _channel;
};


#endif // CALIBRATION_H

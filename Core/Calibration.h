#ifndef CALIBRATION_H
#define CALIBRATION_H


// Project
#include "Channel.h"

//RsaToolbox
#include "Vna.h"

// Qt
#include <QObject>
#include <QSharedPointer>


class Calibration : public QObject
{
    Q_OBJECT
public:
    explicit Calibration(QObject *parent = 0);
    Calibration(const Calibration &other);
    ~Calibration();

    bool isEmpty() const;
    bool isCalGroup() const;
    bool isChannel() const;
    QString calGroup() const;
    uint channel() const;
    QString displayText() const;

    Channel load(RsaToolbox::Vna *vna) const;

    void operator=(const Calibration &other);

signals:
    void error(QString message);
    void changed(QString displayText);

public slots:
    void clear();
    bool setCalGroup(QString group);
    bool setChannel(uint index);

private:
    bool _isCalGroup;
    QString _calGroup;
    uint _channel;
};

bool operator==(const Calibration &rhs, const Calibration &lhs);
bool operator!=(const Calibration &rhs, const Calibration &lhs);


#endif // CALIBRATION_H

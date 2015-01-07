#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H


// Project
#include "Data.h"

// RsaToolbox
#include "General.h"

// Qt
#include <QThread>
#include <QVector>


class CalculateThread : public QThread
{
    Q_OBJECT
public:
    explicit CalculateThread(QObject *parent = 0);
    ~CalculateThread();

    bool isData() const;
    Data *data() const;
    void setData(Data *data);

    bool isReady();
    bool isError() const;

signals:
    void error(QString message);

public slots:

protected:
    virtual void run();

private:
    Data *_data;

    bool hasPorts();
    bool hasCalibrations();
    bool loadCalibrations();
    bool calibrationsMatch();
    bool frequencyIsKnown();
    void calculateFrequency();

    bool moreThanOnePortCalibrated();
    bool portsAreCalibrated();

    bool isPortsLeft() const;
    void getVnaPorts();
    bool portPair(uint &port1, uint &vnaPort1, uint &port2, uint &vnaPort2);
    void calculate(uint port1, uint vnaPort1, uint port2, uint vnaPort2);

    void deleteChannels();
    void deleteFiles();
    void reset();

    bool _isError;

    QVector<uint> _vnaPorts;
    QVector<uint> _calibratedPorts;
    QVector<uint> _portsLeft;

    bool _deleteOuterChannel;
    uint _outerChannel;
    RsaToolbox::VnaCorrections _outerData;

    bool _deleteInnerChannel;
    uint _innerChannel;
    RsaToolbox::VnaCorrections _innerData;

    RsaToolbox::QRowVector _frequency_Hz;
};


#endif // CALCULATETHREAD_H

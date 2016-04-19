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

    bool areFilesWritable();
    bool hasPorts();
    bool hasCalibrations();
    bool loadCalibrations();
    bool isFull12TermErrorCorrections();
    bool calibrationsMatch();
    bool frequencyIsKnown();
    void calculateFrequency();

    bool moreThanOnePortCalibrated();
    bool portsAreCalibrated();

    bool isPortsLeft() const;
    void getVnaPorts();
    bool portPair(uint &port1, uint &vnaPort1, bool &isPort1Matrix, uint &port2, uint &vnaPort2, bool &isPort2Matrix);
    bool calculate(uint port1, uint vnaPort1, uint port2, uint vnaPort2);
    RsaToolbox::NetworkData calculateNetwork(uint port1, uint vnaPort1, uint port2, uint vnaPort2);
    void constructMatrix(RsaToolbox::ComplexMatrix3D &matrix, const RsaToolbox::ComplexRowVector &s11, const RsaToolbox::ComplexRowVector &s21, const RsaToolbox::ComplexRowVector &s22);

    void deleteChannels();
    void deleteFiles();
    void reset();

    bool _isError;

    bool _areMatrices;
    RsaToolbox::Ports _vnaPorts;
    RsaToolbox::Ports _calibratedPorts;
    RsaToolbox::Ports _portsLeft;

    bool _deleteOuterChannel;
    uint _outerChannel;
    RsaToolbox::VnaCorrections _outerData;

    bool _deleteInnerChannel;
    uint _innerChannel;
    RsaToolbox::VnaCorrections _innerData;

    RsaToolbox::QRowVector _frequency_Hz;
};


#endif // CALCULATETHREAD_H

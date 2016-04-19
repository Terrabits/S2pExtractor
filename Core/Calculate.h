#ifndef CALCULATE_H
#define CALCULATE_H


// Project
#include "Data.h"

// RsaToolbox
#include "General.h"

// Qt
#include <QThread>
#include <QVector>


class Calculate
{
//    Q_OBJECT
public:
    explicit Calculate();
    ~Calculate();

//    bool isData() const;
//    Data *data() const;
//    void setData(Data *data);

//    bool isReady();
//    bool isError() const;

//private:
//    Data *_data;

//    bool areFilesWritable();
//    bool hasPorts();
//    bool hasCalibrations();
//    bool loadCalibrations();
//    bool isFull12TermErrorCorrections();
//    bool calibrationsMatch();
//    bool frequencyIsKnown();
//    void calculateFrequency();

//    bool moreThanOnePortCalibrated();
//    bool portsAreCalibrated();

//    bool isPortsLeft() const;
//    void getVnaPorts();
//    bool portPair(uint &port1, uint &vnaPort1, bool &isPort1Matrix, uint &port2, uint &vnaPort2, bool &isPort2Matrix);
//    bool calculate(uint port1, uint vnaPort1, uint port2, uint vnaPort2);
//    RsaToolbox::NetworkData calculateNetwork(uint port1, uint vnaPort1, uint port2, uint vnaPort2);
//    void constructMatrix(RsaToolbox::ComplexMatrix3D &matrix, const RsaToolbox::ComplexRowVector &s11, const RsaToolbox::ComplexRowVector &s21, const RsaToolbox::ComplexRowVector &s22);

//    void deleteChannels();
//    void deleteFiles();
//    void reset();

//    bool _isError;

//    bool _areMatrices;
//    QVector<uint> _vnaPorts;
//    QVector<uint> _calibratedPorts;
//    QVector<uint> _portsLeft;

//    bool _deleteOuterChannel;
//    uint _outerChannel;
//    RsaToolbox::VnaCorrections _outerData;

//    bool _deleteInnerChannel;
//    uint _innerChannel;
//    RsaToolbox::VnaCorrections _innerData;

//    RsaToolbox::QRowVector _frequency_Hz;
};


#endif // CALCULATE_H

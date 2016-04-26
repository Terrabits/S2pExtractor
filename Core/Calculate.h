#ifndef CALCULATE_H
#define CALCULATE_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <NetworkData.h>

// Qt
#include <QVector>


class Calculate
{
//    Q_OBJECT
public:
    explicit Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, uint numberOfTestPorts);
    ~Calculate();

    bool isError(QString &message = QString());
    RsaToolbox::NetworkData result(uint port);

private:
    bool _isError;
    QString _errorMessage;
    QVector<uint> _ports;
    QVector<RsaToolbox::NetworkData> _results;
    void setResult(uint port, RsaToolbox::NetworkData &data);

    // Temporary variables,
    // Helper functions
    uint _numberOfTestPorts;
};


#endif // CALCULATE_H

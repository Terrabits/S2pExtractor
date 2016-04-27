#ifndef CALCULATE_H
#define CALCULATE_H


// Project
#include "CalibrationSource.h"
#include "Corrections.h"

// RsaToolbox
#include <NetworkData.h>
#include <Vna.h>

// Qt
#include <QVector>


class Calculate
{
//    Q_OBJECT
public:
    explicit Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, RsaToolbox::Vna *vna);
    ~Calculate();

    bool isError(QString &message = QString()) const;
    RsaToolbox::NetworkData result(uint port) const;

private:
    bool _isError;
    QString _errorMessage;
    void setError(const QString &message);

    QVector<uint> _ports;
    QVector<RsaToolbox::NetworkData> _results;
    static RsaToolbox::NetworkData processPort1(Corrections &outer, Corrections &inner);
    static RsaToolbox::NetworkData processPort2(Corrections &outer, Corrections &inner);
    void setResult(uint port, RsaToolbox::NetworkData &data);

    // Temporary variables,
    // Helper functions
    uint _numberOfTestPorts;
};


#endif // CALCULATE_H

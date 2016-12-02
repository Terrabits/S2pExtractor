#ifndef CALCULATE_H
#define CALCULATE_H


// Project
#include "CalibrationSource.h"
#include "Corrections.h"
#include "Error.h"

// RsaToolbox
#include <NetworkData.h>
#include <Vna.h>

// Qt
#include <QObject>
#include <QVector>


class Calculate : public QObject
{
    Q_OBJECT
public:
    explicit Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, RsaToolbox::Vna *vna, QObject *parent = 0);
    ~Calculate();

    bool isReady(Error &error);
    bool isError() const;
    Error error() const;

    RsaToolbox::NetworkData result(uint port) const;

signals:
    void progress(int percent);

public slots:
    void run();

private:
    Error _error;
    void setError(Error::Code code, const QString &message);

    CalibrationSource _outerSource;
    CalibrationSource _innerSource;
    QVector<uint> _ports;
    RsaToolbox::Vna *_vna;

    static bool isCommonPorts(QVector<uint> a, QVector<uint> b);

    QVector<RsaToolbox::NetworkData> _results;
    static RsaToolbox::NetworkData processPort1(Corrections &outer, Corrections &inner);
    static RsaToolbox::NetworkData processPort2(Corrections &outer, Corrections &inner);
    void setResult(uint port, RsaToolbox::NetworkData &data);
};


#endif // CALCULATE_H

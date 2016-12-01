#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H


// Project
#include "Calculate.h"
#include "CalibrationSource.h"

// RsaToolbox
#include <NetworkData.h>
#include <Vna.h>

// Qt
#include <QScopedPointer>
#include <QThread>
#include <QVector>


class CalculateThread : public QThread
{
    Q_OBJECT
public:
    explicit CalculateThread(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, RsaToolbox::Vna *vna, QObject *parent = 0);
    ~CalculateThread();

    Calculate *result();

signals:
    void progress(int percent);

protected:
    virtual void run();

private:
    CalibrationSource _outerSource;
    CalibrationSource _innerSource;
    QVector<uint> _ports;
    RsaToolbox::Vna *_vna;

    QScopedPointer<Calculate> _calculate;
};


#endif // CALCULATETHREAD_H

#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H


// Project
#include "Corrections.h"

// RsaToolbox
#include <NetworkData.h>

// Qt
#include <QThread>
#include <QVector>


class CalculateThread : public QThread
{
    Q_OBJECT
public:
    explicit CalculateThread(QObject *parent = 0);
    ~CalculateThread();

    void setOuterCorrections(const Corrections &corrections);
    void setInnerCorrections(const Corrections &corrections);
    void setPorts(QVector<uint> ports);

    bool isError(QString &message = QString());
    QVector<RsaToolbox::NetworkData> results();
protected:
    virtual void run();

private:
    Corrections _outerCorrections;
    Corrections _innerCorrections;
    QVector<uint> _ports;

    QVector<RsaToolbox::NetworkData> _results;
};


#endif // CALCULATETHREAD_H

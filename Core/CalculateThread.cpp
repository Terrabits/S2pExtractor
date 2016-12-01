#include "CalculateThread.h"


// Project
#include "Calculate.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalculateThread::CalculateThread(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, Vna *vna, QObject *parent) :
    QThread(parent),
    _outerSource(outer),
    _innerSource(inner),
    _ports(ports),
    _vna(vna)
{

}
CalculateThread::~CalculateThread() {

}

Calculate *CalculateThread::result() {
    return _calculate.data();
}

void CalculateThread::run() {
    _calculate.reset(new Calculate(_outerSource, _innerSource, _ports, _vna));
    QObject::connect(_calculate.data(), SIGNAL(progress(int)),
                     this, SIGNAL(progress(int)));
    _calculate->run();
    QObject::disconnect(_calculate.data(), SIGNAL(progress(int)),
                        this, SIGNAL(progress(int)));
}

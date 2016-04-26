#include "Calculate.h"


// Project
#include "PortLoop.h"
#include "Channel.h"
#include "Corrections.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calculate::Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, uint numberOfTestPorts)
{
    QVector<uint> allPorts = range(uint(1), numberOfTestPorts);
    PortLoop loop(ports, allPorts);
    while (loop.isUnprocessedPorts()) {
        loop.begin();
        do {
            Corrections _outer(loop.port1(), loop.port2());
            Corrections _inner;
        } while (loop.next());
    }
}
Calculate::~Calculate()
{
    //
}



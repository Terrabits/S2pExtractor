#include "Calculate.h"


// Project
#include "PortLoop.h"
#include "Corrections.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calculate::Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, uint numberOfTestPorts)
{
    PortLoop loop(ports, range(uint(1), numberOfTestPorts));
    while (loop.isUnprocessedPorts()) {
        loop.begin();
        do {
            Corrections _outer;
            Corrections _inner;
        } while (loop.next());
    }
}
Calculate::~Calculate()
{
    //
}



#include "Calculate.h"


// Project
#include "PortLoop.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calculate::Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, RsaToolbox::Vna *vna)
{
    _ports = ports;
    QVector<uint> allPorts = range(uint(1), vna->testPorts());

    _results.resize(ports.size());
    PortLoop loop(ports, allPorts);
    while (loop.isUnprocessedPorts()) {
        loop.begin();
        bool success = false;
        do {
            Corrections outerCorrections(loop.port1(), loop.port2(), outer, vna);
            if (!outerCorrections.isPort1Corrections())
                continue;
            Corrections innerCorrections(loop.port1(), loop.port2(), inner, vna);
            if (!innerCorrections.isPort1Corrections())
                continue;

            // Process Port 1
            success = true;
            loop.usePorts();
            loop.end();
            setResult(loop.port1(), processPort1(outerCorrections, innerCorrections));

            if (!ports.contains(loop.port2()))
                continue;
            if (!outerCorrections.isPort2Corrections())
                continue;
            if (!innerCorrections.isPort2Corrections())
                continue;

            // Process Port 2
            setResult(loop.port2(), processPort2(outerCorrections, innerCorrections));
        } while (loop.next());

        // No corrections found for port 1
        if (!success) {
            QString msg = "Could not complete calculation for port %1";
            msg = msg.arg(loop.port1());
            setError(msg);
            break;
        }
    }
}
Calculate::~Calculate()
{
    //
}

bool Calculate::isError(QString &message) const {
    if (_isError)
        message = _errorMessage;
    else
        message.clear();
    return _isError;
}
NetworkData Calculate::result(uint port) const {
    const int index = _ports.indexOf(port);
    return _results[index];
}

void Calculate::setError(const QString &message) {
    _isError = true;
    _errorMessage = message;
}

NetworkData Calculate::processPort1(Corrections &outer, Corrections &inner) {
    ComplexRowVector denominator = add(outer.reflectionTracking1(), multiplyEach(outer.sourceMatch1(), subtract(inner.directivity1(), outer.directivity1())));
    ComplexRowVector s11 = divideEach(subtract(outer.reflectionTracking1(), inner.directivity1()), denominator);
    ComplexRowVector s22 = subtract(inner.sourceMatch1(), divideEach(multiplyEach(outer.sourceMatch1(), inner.reflectionTracking1()), denominator));
    ComplexRowVector s21Numerator = smoothSqrt(multiplyEach(outer.reflectionTracking1(), inner.reflectionTracking1()));
    ComplexRowVector s21 = divideEach(s21Numerator, denominator);

    NetworkData data;
    data.setPorts(2);
    data.setPoints(outer.frequencies_Hz().size());
    data.x() = outer.frequencies_Hz();
    for (uint i = 0; i < data.points(); i++) {
        data.y()[i][0][0] = s11[i];
        data.y()[i][1][1] = s22[i];
        data.y()[i][0][1] = s21[i];
        data.y()[i][1][0] = s21[i];
    }
    return data;
}
NetworkData Calculate::processPort2(Corrections &outer, Corrections &inner) {
    ComplexRowVector denominator = add(outer.reflectionTracking2(), multiplyEach(outer.sourceMatch2(), subtract(inner.directivity2(), outer.directivity2())));
    ComplexRowVector s11 = divideEach(subtract(outer.reflectionTracking2(), inner.directivity2()), denominator);
    ComplexRowVector s22 = subtract(inner.sourceMatch2(), divideEach(multiplyEach(outer.sourceMatch2(), inner.reflectionTracking2()), denominator));
    ComplexRowVector s21Numerator = smoothSqrt(multiplyEach(outer.reflectionTracking2(), inner.reflectionTracking2()));
    ComplexRowVector s21 = divideEach(s21Numerator, denominator);

    NetworkData data;
    data.setPorts(2);
    data.setPoints(outer.frequencies_Hz().size());
    data.x() = outer.frequencies_Hz();
    for (uint i = 0; i < data.points(); i++) {
        data.y()[i][0][0] = s11[i];
        data.y()[i][1][1] = s22[i];
        data.y()[i][0][1] = s21[i];
        data.y()[i][1][0] = s21[i];
    }
    return data;
}

void Calculate::setResult(uint port, NetworkData &data) {
    const int index = _ports.indexOf(port);
    _results[index] = data;
}

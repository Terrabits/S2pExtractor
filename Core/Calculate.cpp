#include "Calculate.h"


// Project
#include "Channel.h"
#include "PortLoop.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


Calculate::Calculate(CalibrationSource outer, CalibrationSource inner, QVector<uint> ports, RsaToolbox::Vna *vna, QObject *parent) :
    QObject(parent),
    _outerSource(outer),
    _innerSource(inner),
    _ports(ports),
    _vna(vna)
{

}
Calculate::~Calculate()
{
    //
}

bool Calculate::isReady(Error &error) {
    error.clear();

    if (_outerSource.isEmpty()) {
        error.code = Error::Code::OuterCalibration;
        error.message = "No calibration specified";
        return false;
    }
    if (_innerSource.isEmpty()) {
        error.code = Error::Code::InnerCalibration;
        error.message = "No calibration specified";
        return false;
    }

    if (_ports.isEmpty()) {
        error.code = Error::Code::Ports;
        error.message = "must specify at least one port";
        return false;
    }
    if (_ports.contains(0)) {
        error.code = Error::Code::Ports;
        error.message = "must be greater than 0";
        return false;
    }

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    foreach (uint port, _ports) {
        if (!testPorts.contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "Port %1 does not exist";
            error.message = error.message.arg(port);
            return false;
        }
    }

    Channel outer(_outerSource, _vna);
    if (!outer.isReady()) {
        error.code = Error::Code::OuterCalibration;
        error.message = "calibration not found";
        return false;
    }

    Channel inner(_innerSource, _vna);
    if (!inner.isReady()) {
        error.code = Error::Code::InnerCalibration;
        error.message = "calibration not found";
        return false;
    }

    if (outer.frequencies_Hz() != inner.frequencies_Hz()) {
        error.code = Error::Code::Other;
        error.message = "frequency points do not match";
        return false;
    }

    if (outer.corrections().switchMatrices() != inner.corrections().switchMatrices()) {
        error.code = Error::Code::Other;
        error.message = "switch matrix setups don't match";
        return false;
    }

    // No common ports
    if (!isCommonPorts(outer.ports(), inner.ports())) {
        error.code = Error::Code::Other;
        error.message = "no common cal ports";
        return false;
    }

    foreach (uint port, _ports) {
        if (!outer.ports().contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "port %1 not found in %2";
            error.message = error.message.arg(port);
            error.message = error.message.arg(_outerSource.displayText());
            return false;
        }
        if (!inner.ports().contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "port %1 not found in %2";
            error.message = error.message.arg(port);
            error.message = error.message.arg(_innerSource.displayText());
            return false;
        }
    }

    // Else
    return true;
}

bool Calculate::isError() const {
    return _error.isError();
}
Error Calculate::error() const {
    return _error;
}

void Calculate::run() {
    if (!isReady(_error))
        return;

    emit progress(0);

    QVector<uint> allPorts = range(uint(1), _vna->testPorts());

    _results.resize(_ports.size());
    PortLoop loop(_ports, allPorts);
    while (loop.isUnprocessedPorts()) {
        loop.begin();
        bool success = false;
        do {
            Corrections outerCorrections(loop.port1(), loop.port2(), _outerSource, _vna);
            if (!outerCorrections.isPort1Corrections())
                continue;
            Corrections innerCorrections(loop.port1(), loop.port2(), _innerSource, _vna);
            if (!innerCorrections.isPort1Corrections())
                continue;

            // Process Port 1
            success = true;
            loop.usePorts();
            loop.end();
            setResult(loop.port1(), processPort1(outerCorrections, innerCorrections));

            if (!_ports.contains(loop.port2()))
                continue;
            if (!outerCorrections.isPort2Corrections())
                continue;
            if (!innerCorrections.isPort2Corrections())
                continue;

            // Process Port 2
            setResult(loop.port2(), processPort2(outerCorrections, innerCorrections));

            emit progress(loop.percentComplete());
        } while (loop.next());

        // No corrections found for port 1
        if (!success) {
            QString msg = "Could not find corrections for port %1";
            msg = msg.arg(loop.port1());
            setError(Error::Code::Other, msg);
            break;
        }
    }
    emit progress(100);
}

NetworkData Calculate::result(uint port) const {
    const int index = _ports.indexOf(port);
    return _results[index];
}

void Calculate::setError(Error::Code code, const QString &message) {
    _error.code = code;
    _error.message = message;
}

bool Calculate::isCommonPorts(QVector<uint> a, QVector<uint> b) {
    foreach (uint port, a) {
        if (b.contains(port))
            return true;
    }

    return false;
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

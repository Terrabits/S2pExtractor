#include "Calculate.h"


// Project
#include "Channel.h"
#include "commonports.h"
#include "PortLoop.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>
#include <QList>


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
        error.message = "*please specify outer calibration";
        return false;
    }
    if (_innerSource.isEmpty()) {
        error.code = Error::Code::InnerCalibration;
        error.message = "*please specify inner calibration";
        return false;
    }
    if (_outerSource == _innerSource) {
        error.code = Error::Code::Other;
        error.message = "*inner and outer cals must be different";
        return false;
    }

    if (_ports.isEmpty()) {
        error.code = Error::Code::Ports;
        error.message = "*please specify at least one port to extract";
        return false;
    }
    if (_ports.contains(0)) {
        error.code = Error::Code::Ports;
        error.message = "*ports must be greater than 0";
        return false;
    }

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    foreach (uint port, _ports) {
        if (!testPorts.contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "*port %1 does not exist";
            error.message = error.message.arg(port);
            return false;
        }
    }

    Channel::Error channelError;
    Channel outer(_outerSource, _vna);
    if (!outer.isReady(channelError)) {
        error.code = Error::Code::OuterCalibration;
        error.message = channelError.message;
        return false;
    }

    Channel inner(_innerSource, _vna);
    if (!inner.isReady(channelError)) {
        error.code = Error::Code::InnerCalibration;
        error.message = channelError.message;
        return false;
    }

    QVector<uint> _commonPorts = commonPorts(&inner, &outer);
    if (_commonPorts.isEmpty()) {
        error.code = Error::Code::Other;
        error.message = "*no common ports found between calibrations";
        return false;
    }
    foreach (const uint port, _ports) {
        if (!outer.ports().contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "port %1 not calibrated in %2";
            error.message = error.message.arg(port);
            error.message = error.message.arg(_outerSource.displayText());
            return false;
        }
        if (!inner.ports().contains(port)) {
            error.code = Error::Code::Ports;
            error.message = "port %1 not calibrated in %2";
            error.message = error.message.arg(port);
            error.message = error.message.arg(_innerSource.displayText());
            return false;
        }
    }
    if (outer.frequencies_Hz() != inner.frequencies_Hz()) {
        error.code = Error::Code::Other;
        error.message = "*frequency points do not match";
        return false;
    }
    if (outer.corrections().switchMatrices() != inner.corrections().switchMatrices()) {
        error.code = Error::Code::Other;
        error.message = "*switch matrix setups don't match";
        return false;
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
    emit started();
    emit progress(0);
    if (!isReady(_error)) {
        emit progress(100);
        emit finished();
        return;
    }

    QVector<uint> allPorts = range(uint(1), _vna->testPorts());

    _results.resize(_ports.size());
    PortLoop loop(_ports, allPorts);
    while (loop.isUnprocessedPorts()) {
        loop.begin();
        const uint port1 = loop.port1();
        const uint port2 = loop.port2();
        bool port1Processed = false;
        do {
            Corrections outerCorrections(port1, port2, _outerSource, _vna);
            if (!outerCorrections.isPort1Corrections()) {
                continue;
            }
            Corrections innerCorrections(port1, port2, _innerSource, _vna);
            if (!innerCorrections.isPort1Corrections()) {
                continue;
            }

            // Process Port 1
            NetworkData port1Result = processPort1(outerCorrections, innerCorrections);
            if (port1Result.points() == 0) {
                // error in calculation
                continue;
            }

            port1Processed = true;
            _error.clear();
            loop.end();
            setResult(port1, port1Result);
            loop.markPortProcessed(port1);
            // TODO: emit progress

            // Process port 2?
            if (!loop.isPort2Unprocessed()) {
                // already processed
                break;
            }
            if (!_ports.contains(port2)) {
                // not a port of interest
                break;
            }
            if (!outerCorrections.isPort2Corrections()) {
                // no outer corrections
                break;
            }
            if (!innerCorrections.isPort2Corrections()) {
                // no inner corrections
                break;
            }

            // Process Port 2
            NetworkData port2Result
                    = processPort2(outerCorrections, innerCorrections);
            if (port2Result.points() == 0) {
                // Ignore port 2 error.
                _error.clear();
                break;
            }

            // Accept port2 calculation
            setResult(port2, port2Result);
            loop.markPortProcessed(port2);
            // TODO: emit progress
            break;
        } while (loop.next());

        if (!port1Processed) {
            if (!_error.isError()) {
                QString msg = "Could not find corrections for port %1";
                msg = msg.arg(port1);
                setError(Error::Code::Other, msg);
            }
            emit progress(100);
            emit finished();
            return;
        }
    }

    // TODO: emit 100% progress
    emit finished();
}

NetworkData Calculate::result(uint port) const {
    const int index = _ports.indexOf(port);
    return _results[index];
}

void Calculate::setError(Error::Code code, const QString &message) {
    _error.code = code;
    _error.message = message;
}

NetworkData Calculate::processPort1(Corrections &outer, Corrections &inner) {
    const uint             points         = outer.frequencies_Hz().size();
    const QRowVector       frequencies_Hz = outer.frequencies_Hz();
    const ComplexRowVector outerReflTrack = outer.reflectionTracking1();
    const ComplexRowVector innerReflTrack = inner.reflectionTracking1();
    const ComplexRowVector outerSrcMatch  = outer.sourceMatch1();
    const ComplexRowVector innerSrcMatch  = inner.sourceMatch1();
    const ComplexRowVector outerDirect    = outer.directivity1();
    const ComplexRowVector innerDirect    = inner.directivity1();

    // Calculation:
    //   denominator = oRT + (oSM * (iDt - oDt))
    //   s11 =         (iDt - oDt) / denominator
    //   s21 =     sqrt(oRT * iRT) / denominator
    //   s22 = iSM -   (oSM * iRT) / denominator
    ComplexRowVector denominator, s11, s22, s21;
    denominator = add(outerReflTrack, multiplyEach(outerSrcMatch, subtract(innerDirect, outerDirect)));
    if (containsZero(denominator)) {
        setError(Error::Code::Other, "Calculation encountered a divide by zero. Are the calibrations sufficient and plausible?");
        return NetworkData();
    }
    s11 =                         divideEach(    subtract(innerDirect,   outerDirect),                     denominator);
    s21 =                         divideEach(        sqrt(multiplyEach  (outerReflTrack, innerReflTrack)), denominator);
    s22 = subtract(innerSrcMatch, divideEach(multiplyEach(outerSrcMatch, innerReflTrack),                  denominator));

    // TODO
    s21 = smoothPhase(s21);

    NetworkData data;
    data.setPorts(2);
    data.setPoints(points);
    data.x() = frequencies_Hz;
    for (uint i = 0; i < data.points(); i++) {
        data.y()[i][0][0] = s11[i];
        data.y()[i][0][1] = s21[i];
        data.y()[i][1][0] = s21[i];
        data.y()[i][1][1] = s22[i];
    }
    return data;
}
NetworkData Calculate::processPort2(Corrections &outer, Corrections &inner) {
    const uint             points         = outer.frequencies_Hz().size();
    const QRowVector       frequencies_Hz = outer.frequencies_Hz();
    const ComplexRowVector outerReflTrack = outer.reflectionTracking2();
    const ComplexRowVector innerReflTrack = inner.reflectionTracking2();
    const ComplexRowVector outerSrcMatch  = outer.sourceMatch2();
    const ComplexRowVector innerSrcMatch  = inner.sourceMatch2();
    const ComplexRowVector outerDirect    = outer.directivity2();
    const ComplexRowVector innerDirect    = inner.directivity2();

    // Calculation:
    //   denominator = oRT + (oSM * (iDt - oDt))
    //   s11 =         (iDt - oDt) / denominator
    //   s21 =     sqrt(oRT * iRT) / denominator
    //   s22 = iSM -   (oSM * iRT) / denominator
    ComplexRowVector denominator, s11, s22, s21;
    denominator = add(outerReflTrack, multiplyEach(outerSrcMatch, subtract(innerDirect, outerDirect)));
    if (containsZero(denominator)) {
        setError(Error::Code::Other, "Calculation encountered a divide by zero. Are the calibrations sufficient and plausible?");
        return NetworkData();
    }
    s11 =                         divideEach(    subtract(innerDirect,   outerDirect),                     denominator);
    s21 =                         divideEach(        sqrt(multiplyEach  (outerReflTrack, innerReflTrack)), denominator);
    s22 = subtract(innerSrcMatch, divideEach(multiplyEach(outerSrcMatch, innerReflTrack),                  denominator));

    // TODO:
    s21 = smoothPhase(s21);

    NetworkData data;
    data.setPorts(2);
    data.setPoints(points);
    data.x() = frequencies_Hz;
    for (uint i = 0; i < data.points(); i++) {
        data.y()[i][0][0] = s11[i];
        data.y()[i][0][1] = s21[i];
        data.y()[i][1][0] = s21[i];
        data.y()[i][1][1] = s22[i];
    }
    return data;
}

void Calculate::setResult(uint port, NetworkData &data) {
    const int index = _ports.indexOf(port);
    _results[index] = data;
}

bool Calculate::containsZero(const ComplexRowVector &values) {
    foreach (const ComplexDouble value, values) {
        if (abs(value) == 0) {
            return true;
        }
    }

    return false;
}

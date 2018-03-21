#include "Corrections.h"


// RsaToolbox
using namespace RsaToolbox;


Corrections::Corrections(uint port1, Ports ports, CalibrationSource source, Vna *vna) :
    _port1(port1),
    _port2(0)
{
    int index = ports.indexOf(port1);
    if (index != -1) {
        ports.remove(index);
    }

    vna->settings().errorDisplayOff();
    foreach (uint port2, ports) {
        Corrections attempt(port1, port2, source, vna);
        if (attempt.isPort1Corrections()) {
            (*this) = attempt;
            // clear port 2
            _port2 = 0;
            _directivity2       .clear();
            _reflectionTracking2.clear();
            _sourceMatch2       .clear();
            break;
        }
    }
    vna->settings().errorDisplayOn();
}
Corrections::Corrections(uint port1, uint port2, CalibrationSource source, Vna *vna) :
    _port1(port1),
    _port2(port2)
{
    Channel channel(source, vna);
    if (!channel.isReady()) {
        return;
    }

    VnaCorrections corrections
                    = channel.corrections();
    _frequencies_Hz = corrections.frequencies_Hz();
    if (_frequencies_Hz.isEmpty()) {
        vna->clearStatus();
        vna->isError();
        return;
    }

    uint switchMatrices = corrections.switchMatrices();

    // No switch matrices:
    // logical == physical
    if (!switchMatrices) {
        _directivity1        = corrections.directivity       (port2, port1);
        _reflectionTracking1 = corrections.reflectionTracking(port2, port1);
        _sourceMatch1        = corrections.sourceMatch       (port2, port1);

        _directivity2        = corrections.directivity       (port1, port2);
        _reflectionTracking2 = corrections.reflectionTracking(port1, port2);
        _sourceMatch2        = corrections.sourceMatch       (port1, port2);
        vna->isError();
        vna->clearStatus();
        return;
    }

    // Switch matrix
    // Find switch matrix path
    // with corrections
    // Logical port -> Physical vna port(s)
    QVector<uint> port1VnaPorts = findVnaPorts(channel, _port1);
    QVector<uint> port2VnaPorts = findVnaPorts(channel, _port2);
    foreach (uint vnaPort1, port1VnaPorts) {
        foreach (uint vnaPort2, port2VnaPorts) {
            if (vnaPort1 == vnaPort2) {
                continue;
            }

            _directivity1        = corrections.directivity       (_port2, vnaPort2, _port1, vnaPort1);
            _reflectionTracking1 = corrections.reflectionTracking(_port2, vnaPort2, _port1, vnaPort1);
            _sourceMatch1        = corrections.sourceMatch       (_port2, vnaPort2, _port1, vnaPort1);
            _directivity2        = corrections.directivity       (_port1, vnaPort1, _port2, vnaPort2);
            _reflectionTracking2 = corrections.reflectionTracking(_port1, vnaPort1, _port2, vnaPort2);
            _sourceMatch2        = corrections.sourceMatch       (_port1, vnaPort1, _port2, vnaPort2);
            vna->isError();
            vna->clearStatus();
            if (isPort1Corrections()) {
                return;
            }

            // try next set of ports
            clearCorrections();
        }
    }
}
Corrections::Corrections(const Corrections &other) :
    _port1(other._port1),
    _port2(other._port2),
    _frequencies_Hz(other._frequencies_Hz),
    _directivity1(other._directivity1),
    _reflectionTracking1(other._reflectionTracking1),
    _sourceMatch1(other.sourceMatch1()),
    _directivity2(other._directivity2),
    _reflectionTracking2(other._reflectionTracking2),
    _sourceMatch2(other._sourceMatch2)
{
    //
}
Corrections::~Corrections()
{
    //
}

QRowVector Corrections::frequencies_Hz() const {
    return _frequencies_Hz;
}

uint Corrections::port1() const {
    return _port1;
}
bool Corrections::isPort1Corrections() const {
    if (_directivity1.empty()) {
        return false;
    }
    const int points = _frequencies_Hz.size();

    if (_directivity1.size()        != points) {
        return false;
    }
    if (_reflectionTracking1.size() != points) {
        return false;
    }
    if (_sourceMatch1.size()        != points) {
        return false;
    }
    return true;
}
ComplexRowVector Corrections::directivity1() const {
    return _directivity1;
}
ComplexRowVector Corrections::reflectionTracking1() const {
    return _reflectionTracking1;
}
ComplexRowVector Corrections::sourceMatch1() const {
    return _sourceMatch1;
}

bool Corrections::isPort2() const {
    return _port2 != 0;
}
uint Corrections::port2() const {
    return _port2;
}
bool Corrections::isPort2Corrections() const {
    if (_directivity2.empty()) {
        return false;
    }
    const int points = _frequencies_Hz.size();

    if (_directivity2.size()        != points) {
        return false;
    }
    if (_reflectionTracking2.size() != points) {
        return false;
    }
    if (_sourceMatch2.size()        != points) {
        return false;
    }
    return true;
}
ComplexRowVector Corrections::directivity2() const {
    return _directivity2;
}
ComplexRowVector Corrections::reflectionTracking2() const {
    return _reflectionTracking2;
}
ComplexRowVector Corrections::sourceMatch2() const {
    return _sourceMatch2;
}

void Corrections::operator=(const Corrections &other) {
    _port1 = other._port1;
    _port2 = other._port2;
    _frequencies_Hz = other._frequencies_Hz;
    _directivity1 = other._directivity1;
    _reflectionTracking1 = other._reflectionTracking1;
    _sourceMatch1 = other._sourceMatch1;
    _directivity2 = other._directivity2;
    _reflectionTracking2 = other._reflectionTracking2;
    _sourceMatch2 = other._sourceMatch2;
}

Ports Corrections::findVnaPorts(Channel &channel, uint logicalPort) const {
    Ports ports;
    const uint switchMatrices = channel.corrections().switchMatrices();
    if (!switchMatrices) {
        ports << logicalPort;
        return ports;
    }
    else if (channel.corrections().testPortToVnaMap().contains(logicalPort)) {
        ports << channel.corrections().testPortToVnaMap()[logicalPort];
        return ports;
    }
    else {
        for (uint i = 1; i <= switchMatrices; i++) {
            if (channel.corrections().testPortToSwitchMatrixMap(i).contains(logicalPort)) {
                ports << channel.corrections().switchMatrixToVnaPortMap(i).values().toVector();
                return ports;
            }
        }
    }

    // Else: Could not find
    return ports;
}

void Corrections::clearCorrections() {
    _frequencies_Hz.clear();

    _directivity1       .clear();
    _reflectionTracking1.clear();
    _sourceMatch1       .clear();

    _directivity2       .clear();
    _reflectionTracking2.clear();
    _sourceMatch2       .clear();
}

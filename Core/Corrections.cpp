#include "Corrections.h"


// RsaToolbox
using namespace RsaToolbox;


Corrections::Corrections(uint port1, Ports ports, CalibrationSource source, Vna *vna) :
    _port1(port1),
    _port2(0)
{
    int index = ports.indexOf(port1);
    if (index != -1)
        ports.remove(index);

    vna->isError();
    vna->clearStatus();
    vna->settings().errorDisplayOff();
    foreach (uint port2, ports) {
        Corrections attempt(port1, port2, source, vna);
        if (attempt.isPort1Corrections()) {
            (*this) = attempt;
            _port2 = 0;
            _directivity2.clear();
            _reflectionTracking2.clear();
            _sourceMatch2.clear();
            break;
        }
    }
    vna->isError();
    vna->clearStatus();
    vna->settings().errorDisplayOn();
}
Corrections::Corrections(uint port1, uint port2, CalibrationSource source, Vna *vna) :
    _port1(port1),
    _port2(port2)
{
    Channel channel(source, vna);
    _frequencies_Hz = channel.frequencies_Hz();

    uint switchMatrices = channel.corrections().switchMatrices();

    // Logical port -> Physical vna port(s)
    QVector<uint> port1VnaPorts = findVnaPorts(channel, _port1);
    QVector<uint> port2VnaPorts = findVnaPorts(channel, _port2);

    // No switch matrices:
    // logical == physical
    if (!switchMatrices) {
        _directivity1 = channel.corrections().directivity(port2, port1);
        _reflectionTracking1 = channel.corrections().reflectionTracking(port2, port1);
        _sourceMatch1 = channel.corrections().sourceMatch(port2, port1);

        _directivity2 = channel.corrections().directivity(port1, port2);
        _reflectionTracking2 = channel.corrections().reflectionTracking(port1, port2);
        _sourceMatch2 = channel.corrections().sourceMatch(port1, port2);
        return;
    }

    // Switch matrix
    // Find switch matrix path
    // with corrections
    foreach (uint vnaPort1, port1VnaPorts) {
        foreach (uint vnaPort2, port2VnaPorts) {
            if (vnaPort1 == vnaPort2)
                continue;

            _directivity1 = channel.corrections().directivity(_port2, vnaPort2, _port1, vnaPort1);
            _reflectionTracking1 = channel.corrections().reflectionTracking(_port2, vnaPort2, _port1, vnaPort1);
            _sourceMatch1 = channel.corrections().sourceMatch(_port2, vnaPort2, _port1, vnaPort1);
            _directivity2 = channel.corrections().directivity(_port1, vnaPort1, _port2, vnaPort2);
            _reflectionTracking2 = channel.corrections().reflectionTracking(_port1, vnaPort1, _port2, vnaPort2);
            _sourceMatch2 = channel.corrections().sourceMatch(_port1, vnaPort1, _port2, vnaPort2);

            if (isPort1Corrections()) {
                return;
            }
            else {
                _directivity1.clear();
                _reflectionTracking1.clear();
                _sourceMatch1.clear();
                _directivity2.clear();
                _reflectionTracking2.clear();
                _sourceMatch2.clear();
            }
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
    const int size = _directivity1.size();
    if (size == 0)
        return false;
    if (_reflectionTracking1.size() != size)
        return false;
    if (_sourceMatch1.size() != size)
        return false;

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
    if (!isPort2())
        return false;

    const int size = _directivity2.size();
    if (size == 0)
        return false;
    if (_reflectionTracking2.size() != size)
        return false;
    if (_sourceMatch2.size() != size)
        return false;

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

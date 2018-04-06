#include "PortLoop.h"

PortLoop::PortLoop(QVector<uint> portsToProcess, QVector<uint> allPorts) :
    _port1(0),
    _port2(0)
{
    _unprocessedPorts = portsToProcess;
//    _processedPorts.clear();
    foreach (const uint port, allPorts) {
        if (!portsToProcess.contains(port))
            _remainderPorts.append(port);
    }
}
PortLoop::~PortLoop()
{

}

bool PortLoop::isUnprocessedPorts() const {
    return !_unprocessedPorts.isEmpty();
}
void PortLoop::begin() {
    _portList.clear();
    _portList += _unprocessedPorts;
    _portList += _processedPorts;
    _portList += _remainderPorts;

    _port1 = _portList.takeFirst();
    _port2 = _portList.takeFirst();
}
uint PortLoop::port1() const {
    return _port1;
}
bool PortLoop::isPort2Unprocessed() const {
    return isPortUnprocessed(port2());
}
uint PortLoop::port2() const {
    return _port2;
}
bool PortLoop::next() {
    if (_portList.isEmpty())
        return false;

    _port2 = _portList.takeFirst();
    return true;
}
void PortLoop::markBothPortsProcessed() {
    markPortProcessed(port1());
    markPortProcessed(port2());
}
void PortLoop::end() {
    _portList.clear();
}

bool PortLoop::isPortUnprocessed(uint port) const {
    return _unprocessedPorts.contains(port);
}
void PortLoop::markPortProcessed(uint port) {
    if (!isPortUnprocessed(port))
        return;

    int _index = _unprocessedPorts.indexOf(port);
    _unprocessedPorts.remove(_index);
    _processedPorts.append(port);
}

#include "PortLoop.h"

PortLoop::PortLoop(QVector<uint> portsToProcess, QVector<uint> allPorts)
{
    _unprocessedPorts = portsToProcess;
//    _processedPorts.clear();
    foreach (uint port, allPorts) {
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
void PortLoop::beginLoop() {
    _portList.clear();
    _portList += _unprocessedPorts;
    _portList += _processedPorts;
    _portList += _remainderPorts;

    _port1 = _portList.takeFirst();
}
uint PortLoop::port1() const {
    return _port1;
}
bool PortLoop::isPort2Unprocessed() const {
    return isPortUnprocessed(port2());
}
uint PortLoop::port2() const {
    return _portList.first();
}
bool PortLoop::next() {
    if (_portList.isEmpty())
        return false;

    _portList.removeFirst();
    return true;
}
void PortLoop::usePorts() {
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
    int _index = _unprocessedPorts.indexOf(port);
    if (_index >= 0) {
        _unprocessedPorts.remove(_index);
        _processedPorts.append(port);
    }
}

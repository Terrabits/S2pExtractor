#include "commonports.h"
#include <algorithm>

QVector<uint> commonPorts(Channel *outerChannel, Channel *innerChannel) {
    QVector<uint> ports;
    if (!outerChannel->isReady()) {
        return ports;
    }
    if (!innerChannel->isReady()) {
        return ports;
    }
    const QVector<uint> outerPorts = outerChannel->ports();
    const QVector<uint> innerPorts = innerChannel->ports();
    foreach (const uint port, outerPorts) {
        if (innerPorts.contains(port)) {
            ports << port;
        }
    }
    foreach (const uint port, innerPorts) {
        if (outerPorts.contains(port)) {
            ports << port;
        }
    }
    std::sort(ports.begin(), ports.end());
    return ports;
}

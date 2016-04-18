#include "Port.h"

Port::Port(uint logicalPort, RsaToolbox::VnaCorrections &corrections) :
    _logicalPort(logicalPort),
    _corrections(corrections)
{
}

Port::~Port() {

}

bool Port::isSwitchMatrixPort() const {
    return !_corrections.isVnaPort(_logicalPort);
}
uint Port::logicalPort() const {
    return _logicalPort;
}
uint Port::physicalVnaPort() const {
    if (isSwitchMatrixPort(_logicalPort)) {
        return;
    }
    else {
        return;
    }
}
uint Port::switchMatrixInputPort() const {
    if (!isSwitchMatrixPort()) {
        return 0;
    }
    else {
        return 1;
    }
}
uint Port::switchMatrixOutputPort() const {

}

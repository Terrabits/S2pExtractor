#ifndef PORT_H
#define PORT_H


// RsaToolbox
#include <VnaCorrections.h>

class Port
{
public:
    Port(uint logicalPort, RsaToolbox::VnaCorrections &corrections);
    ~Port();

    bool isSwitchMatrixPort() const;
    uint logicalPort() const;
    uint physicalVnaPort() const;
    uint switchMatrixInputPort() const;
    uint switchMatrixOutputPort() const;


private:
    uint _logicalPort;
    mutable RsaToolbox::VnaCorrections _corrections;
};

#endif // PORT_H

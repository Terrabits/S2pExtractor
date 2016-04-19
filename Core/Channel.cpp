#include "Channel.h"


// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;


Channel::Channel(uint channel, Vna *vna) :
    _isManaged(false),
    _channel(channel),
    _vna(vna)
{
    //
}
Channel::Channel(QString calGroup, Vna *vna) :
    _isManaged(true),
    _channel(0),
    _vna(vna)
{
    _channel = _vna->createChannel();
    VnaChannel channel = _vna->channel(_channel);
    channel.setCalGroup(calGroup);
}

Channel::~Channel()
{
    cleanup();
}

bool Channel::isChannel() const {
    return _vna->isChannel(_channel);
}
bool Channel::isCalibrated() const {
    return _vna->channel(_channel).isCalibrated();
}
bool Channel::isReady() const {
    return isChannel() && isCalibrated();
}
VnaCorrections Channel::corrections() const {
    return _vna->channel(_channel).corrections();
}

void Channel::cleanup() {
    if (_isManaged && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _isManaged = false;
        _channel = 0;
    }
}

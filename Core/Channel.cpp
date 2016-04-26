#include "Channel.h"


// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;


Channel::Channel(CalibrationSource source, Vna *vna) :
    _isManaged(false),
    _channel(0),
    _vna(vna)
{
    if (source.isChannel()) {
        _channel = source.channel();
    }
    else if (source.isCalGroup()) {
        _isManaged = true;
        _channel = _vna->createChannel();
        const QString calGroup = source.calGroup();
        _vna->channel(_channel).setCalGroup(calGroup);
    }
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

QVector<uint> Channel::ports() const {
    return corrections().ports();
}
QRowVector Channel::frequencies_Hz() const {
    return corrections().frequencies_Hz();
}

void Channel::cleanup() {
    if (_isManaged && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _isManaged = false;
        _channel = 0;
    }
}

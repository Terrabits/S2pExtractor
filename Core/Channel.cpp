#include "Channel.h"


// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;


Channel::Channel(CalibrationSource source, Vna *vna) :
    _vna(vna)
{
    clear();
    if (source.isChannel())
        setChannel(source.channel());
    if (source.isCalGroup())
        setCalGroup(source.calGroup());
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
    if (!isChannel())
        return false;
    if (!isCalibrated())
        return false;
    if (_source.isCalGroup()) {
        const QString calGroup = _vna->channel(_channel).calGroup();
        if (calGroup != _source.calGroup())
            return false;
    }

    return true;
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

void Channel::clear() {
    _isManaged = false;
    _channel   = 0;
    _source.clear();
}

bool Channel::vnaHasChannel(uint c) {
    return _vna->isChannel(c);
}
bool Channel::vnaHasCalGroup(const QString &calGroup) {
    return _vna->isCalGroup(calGroup);
}
void Channel::setChannel(uint c) {
    clear();
    if (!vnaHasChannel(c))
        return;

    _source.setChannel(c);
    _channel = c;
}
void Channel::setCalGroup(const QString &calGroup) {
    clear();
    if (!vnaHasCalGroup(calGroup))
        return;

    _source.setCalGroup(calGroup);

    _isManaged = true;
    _channel = _vna->createChannel();
    _vna->channel(_channel).setCalGroup(calGroup);
}
void Channel::cleanup() {
    if (_isManaged && _vna->isChannel(_channel)) {
        _vna->deleteChannel(_channel);
        _isManaged = false;
        _channel = 0;
    }
}

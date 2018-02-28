#include "Channel.h"

// RsaToolbox
#include <VnaChannel.h>
using namespace RsaToolbox;

// Error class
Channel::Error::Error() {
    clear();
}
bool Channel::Error::isError() const {
    return code != Error::Code::None;
}
void Channel::Error::clear() {
    code = Error::Code::None;
    message.clear();
}

// Channel
Channel::Channel(CalibrationSource source, Vna *vna)
{
    clear();
    _vna    = vna;
    if (source.isChannel()) {
        processChannel(source.channel());
        return;
    }
    if (source.isCalGroup()) {
        processCalGroup(source.calGroup());
        return;
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
    Error error;
    return isReady(error);
}
bool Channel::isReady(Error &error) const {
    error.clear();
    if (_source.isEmpty()) {
        error.code = Error::Code::NoSource;
        error.message = "*please specify a calibration source";
        return false;
    }
    if (_source.isChannel() && !isChannel()) {
        error.code = Error::Code::ChannelMissing;
        error.message = "*could not find channel %1";
        error.message = error.message.arg(_source.channel());
        return false;
    }
    if (_source.isCalGroup() && !_vna->isCalGroup(_source.calGroup())) {
        error.code = Error::Code::CalGroupMissing;
        error.message = "*cal group \'%1\' not found";
        error.message = error.message.arg(_source.calGroup());
        return false;
    }
    if (!isCalibrated()) {
        error.code = Error::Code::NoCalData;
        if (_source.isChannel()) {
            error.message = "*no cal data in channel %1";
            error.message = error.message.arg(_source.channel());
            return false;
        }
        else {
            error.message = "*no cal data found in cal group \'%1\'";
            error.message = error.message.arg(_source.calGroup());
            return false;
        }
    }

    // No error found
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
void Channel::processChannel(uint c) {
    _source.setChannel(c);
    if (!_vna->isChannel(c)) {
        return;
    }
    _channel = c;
}
void Channel::processCalGroup(const QString &calGroup) {
    _source.setCalGroup(calGroup);
    if (!_vna->isCalGroup(calGroup)) {
        return;
    }
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

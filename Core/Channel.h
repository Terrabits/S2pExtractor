#ifndef CHANNEL_H
#define CHANNEL_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>
#include <VnaCorrections.h>

// Qt
#include <Qt>
#include <QString>


class Channel
{
public:
    Channel(CalibrationSource source, RsaToolbox::Vna *vna);
    ~Channel();

    bool isChannel() const;
    bool isCalibrated() const;
    bool isReady() const;
    RsaToolbox::VnaCorrections corrections() const;

    // Unnecessary?
    QVector<uint> ports() const;
    RsaToolbox::QRowVector frequencies_Hz() const;

private:
    bool _isManaged;
    uint _channel;
    CalibrationSource _source;
    mutable RsaToolbox::Vna *_vna;

    void clear();
    bool vnaHasChannel(uint c);
    bool vnaHasCalGroup(const QString &calGroup);
    void setChannel(uint c);
    void setCalGroup(const QString &calGroup);
    void cleanup();
};

#endif // CHANNEL_H

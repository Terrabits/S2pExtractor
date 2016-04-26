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
    mutable RsaToolbox::Vna *_vna;

    void cleanup();
};

#endif // CHANNEL_H

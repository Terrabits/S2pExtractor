#ifndef CHANNEL_H
#define CHANNEL_H


// RsaToolbox
#include <Vna.h>
#include <VnaCorrections.h>

// Qt
#include <Qt>
#include <QString>


class Channel
{
public:
    Channel(uint channel, RsaToolbox::Vna *vna);
    Channel(QString calGroup, RsaToolbox::Vna *vna);
    ~Channel();

    bool isChannel() const;
    bool isCalibrated() const;
    bool isReady() const;
    RsaToolbox::VnaCorrections corrections() const;

private:
    bool _isManaged;
    uint _channel;
    mutable RsaToolbox::Vna *_vna;

    void cleanup();
};

#endif // CHANNEL_H

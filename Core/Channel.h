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

    class Error {
    public:
        enum Code {
            NoSource,
            ChannelMissing,
            CalGroupMissing,
            CalGroupProblem,
            NoCalData,
            None
        };
        Error();

        Code  code;
        QString message;

        bool isError() const;
        void clear();
    };

    bool isChannel() const;
    bool isCalibrated() const;
    bool isReady() const;
    bool isReady(Error &error) const;
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
    void processChannel(uint c);
    void processCalGroup(const QString &calGroup);
    void cleanup();
};

#endif // CHANNEL_H

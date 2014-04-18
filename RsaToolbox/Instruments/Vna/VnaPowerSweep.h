#ifndef VnaPowerSweep_H
#define VnaPowerSweep_H


// RsaToolbox
#include "Definitions.h"
// Etc

// Qt
#include <QObject>
#include <QScopedPointer>


namespace RsaToolbox {
class Vna;
class VnaChannel;

class VnaPowerSweep : public QObject
{
private: Q_OBJECT

public:
    explicit VnaPowerSweep(QObject *parent = 0);
    VnaPowerSweep(VnaPowerSweep &other);
    VnaPowerSweep(Vna *vna, VnaChannel *channel, QObject *parent = 0);
    VnaPowerSweep(Vna *vna, uint channelIndex, QObject *parent = 0);

    uint points();
    void setPoints(uint numberOfPoints);
    double start_dBm();
    void setStart(double power_dBm);
    double stop_dBm();
    void setStop(double power_dBm);
    QVector<double> powers_dBm();
    double frequency_Hz();
    void setFrequency(double frequency, SiPrefix prefix = NO_PREFIX);
    double ifBandwidth_Hz();
    void setIfbandwidth(double bandwidth, SiPrefix prefix = NO_PREFIX);

    void operator=(VnaPowerSweep const &other);

//    void moveToThread(QThread *thread);

private:
    Vna *_vna;
    QScopedPointer<Vna> placeholder;
    QScopedPointer<VnaChannel> _channel;
    uint _channelIndex;

    bool isFullyInitialized() const;
};
}

#endif // VnaPowerSweep_H

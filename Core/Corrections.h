#ifndef CORRECTIONS_H
#define CORRECTIONS_H


// Project
#include "CalibrationSource.h"
#include "Channel.h"

// RsaToolbox
#include <Vna.h>
#include <VnaCorrections.h>

// Qt
#include <Qt>
#include <QVector>


class Corrections
{
public:
    Corrections(uint port1, RsaToolbox::Ports ports, CalibrationSource source, RsaToolbox::Vna *vna);
    Corrections(uint port1, uint port2, CalibrationSource source, RsaToolbox::Vna *vna);
    Corrections(const Corrections &other);
    ~Corrections();

    RsaToolbox::QRowVector frequencies_Hz() const;

    // Port 1
    uint port1() const;
    bool isPort1Corrections() const;
    RsaToolbox::ComplexRowVector directivity1       () const;
    RsaToolbox::ComplexRowVector reflectionTracking1() const;
    RsaToolbox::ComplexRowVector sourceMatch1       () const;

    // Port 2
    bool isPort2() const;
    uint port2  () const;
    bool isPort2Corrections() const;
    RsaToolbox::ComplexRowVector directivity2       () const;
    RsaToolbox::ComplexRowVector reflectionTracking2() const;
    RsaToolbox::ComplexRowVector sourceMatch2       () const;

    void operator=(const Corrections &other);

private:
    uint _port1;
    uint _port2;
    RsaToolbox::Ports findVnaPorts(Channel &channel, uint logicalPort) const;

    RsaToolbox::QRowVector _frequencies_Hz;

    // Port 1
    RsaToolbox::ComplexRowVector _directivity1;
    RsaToolbox::ComplexRowVector _reflectionTracking1;
    RsaToolbox::ComplexRowVector _sourceMatch1;

    // Port 2
    RsaToolbox::ComplexRowVector _directivity2;
    RsaToolbox::ComplexRowVector _reflectionTracking2;
    RsaToolbox::ComplexRowVector _sourceMatch2;

    void clearCorrections();
};

#endif // CORRECTIONS_H

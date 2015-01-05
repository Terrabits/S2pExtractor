#ifndef DATA_H
#define DATA_H


// Project
#include "Calibration.h"
#include "Ports.h"

// RsaToolbox
#include "Keys.h"
#include "Vna.h"

// Qt
#include <QSharedPointer>


class Data
{
public:
    Data();
    ~Data();

    bool isKeys() const;
    RsaToolbox::Keys *keys() const;
    void setKeys(RsaToolbox::Keys *keys);

    bool isVna() const;
    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    SharedCalibration outerCalibration() const;
    SharedCalibration innerCalibration() const;
    SharedPorts ports() const;

private:
    RsaToolbox::Keys *_keys;
    RsaToolbox::Vna *_vna;
    SharedCalibration _outerCal;
    SharedCalibration _innerCal;
    SharedPorts _ports;
};

#endif // DATA_H

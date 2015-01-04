#ifndef DATA_H
#define DATA_H


// Project
#include "Calibration.h"

// RsaToolbox
#include "Vna.h"


class Data
{
public:
    Data();
    ~Data();

private:
    RsaToolbox::Vna *_vna;
    SharedCalibration outerCal;
    SharedCalibration innerCal;
    QVector<uint> ports;
};

#endif // DATA_H

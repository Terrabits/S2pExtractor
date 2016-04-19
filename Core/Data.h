#ifndef DATA_H
#define DATA_H


// Project
#include "Calibration.h"

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


    Calibration outerCalibration() const;
    Calibration innerCalibration() const;
    void setOuterCalibration(const Calibration &calibration);
    void setInnerCalibration(const Calibration &calibration);

    QVector<uint> ports() const;
    void setPorts(QVector<uint> ports);

    QStringList filenames() const;
    void setFilenames(QStringList filenames);

private:
    RsaToolbox::Keys *_keys;
    RsaToolbox::Vna *_vna;
    Calibration _outerCal;
    Calibration _innerCal;
    QVector<uint> _ports;
    QStringList _filenames;
};

#endif // DATA_H

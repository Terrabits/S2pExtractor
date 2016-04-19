#include "Data.h"

// Project
//

// RsaToolbox
using namespace RsaToolbox;


Data::Data()
{
    //
}
Data::~Data()
{
    //
}

bool Data::isKeys() const {
    return _keys != NULL;
}
Keys *Data::keys() const {
    return _keys;
}
void Data::setKeys(RsaToolbox::Keys *keys) {
    _keys = keys;
}

bool Data::isVna() const {
    return _vna != NULL;
}
Vna *Data::vna() const {
    return _vna;
}
void Data::setVna(RsaToolbox::Vna *vna) {
    _vna = vna;
}

Calibration Data::outerCalibration() const {
    return _outerCal;
}
Calibration Data::innerCalibration() const {
    return _innerCal;
}
void Data::setOuterCalibration(const Calibration &calibration) {
    _outerCal = calibration;
}
void Data::setInnerCalibration(const Calibration &calibration) {
    _innerCal = calibration;
}

QVector<uint> Data::ports() const {
    return _ports;
}
void Data::setPorts(QVector<uint> ports) {
    _ports = ports;
}

QStringList Data::filenames() const {
    return _filenames;
}
void Data::setFilenames(QStringList filenames) {
    _filenames = filenames;
}

#include "Data.h"

// Project
//

// RsaToolbox
using namespace RsaToolbox;


Data::Data()
{
    _outerCal = newCalibration();
    _innerCal = newCalibration();
    _ports = newPorts();
}
Data::~Data()
{

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

SharedCalibration Data::outerCalibration() const {
    return _outerCal;
}
SharedCalibration Data::innerCalibration() const {
    return _innerCal;
}
SharedPorts Data::ports() const {
    return _ports;
}
QStringList &Data::filePathNames() {
    return _pathNames;
}

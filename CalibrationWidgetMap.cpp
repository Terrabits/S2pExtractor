#include "CalibrationWidgetMap.h"


// RsaToolbox
using namespace RsaToolbox;

CalibrationWidgetMap::CalibrationWidgetMap(QObject *parent) : QObject(parent)
{

}

CalibrationWidgetMap::~CalibrationWidgetMap()
{

}


Vna CalibrationWidgetMap::vna() const {
    return _vna;
}
void CalibrationWidgetMap::setVna(Vna *vna) {
    _vna = vna;
}

QLineEdit *CalibrationWidgetMap::lineEdit() const {
    return _lineEdit;
}
void CalibrationWidgetMap::setLineEdit(QLineEdit *edit) {
    if (_lineEdit == edit)
        return;

    disconnectView();
    _lineEdit = edit;
    connectView();
    udpate();

}
QPushButton CalibrationWidgetMap::pushButton() const {
    return _pushButton;
}
void CalibrationWidgetMap::setPushButton(QPushButton *button) {
    if (_pushButton == button)
        return;

    disconnectView();
    _pushButton = button;
    connectView();
}

SharedCalibration CalibrationWidgetMap::calibration() const {
    return _calibration;
}
void CalibrationWidgetMap::setCalibration(SharedCalibration calibration) {
    if (_calibration == calibration)
        return;

    _calibration = calibration;
}

void CalibrationWidgetMap::getCalibration() {
    if (!isVna())
        return;

    _dialog.exec(_vna);
    if (_dialog.isOkClicked()) {
        *_calibration = _dialog.result();
    }
}



void CalibrationWidgetMap::connectView() {
    if (isLineEdit()) {
        QObject::connect(_calibration.data(), SIGNAL(changed(QString)),
                         _lineEdit, SLOT(setText(QString)));
    }
    if (isPushButton()) {
        QObject::connect(_pushButton, SIGNAL(clicked()),
                         this, SLOT(getCalibration()));
    }
}
void CalibrationWidgetMap::disconnectView() {
    if (isLineEdit()) {
        QObject::disconnect(_calibration.data(), SIGNAL(changed(QString)),
                         _lineEdit, SLOT(setText(QString)));
    }
    if (isPushButton()) {
        QObject::disconnect(_pushButton, SIGNAL(clicked()),
                         this, SLOT(getCalibration()));
    }
}


#include "CalibrationWidgetMap.h"


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalibrationWidgetMap::CalibrationWidgetMap(QWidget *parent) :
    _dialog(parent),
    QObject(parent)
{
    _calibration = newCalibration();
    _lineEdit = NULL;
    _pushButton = NULL;
}

CalibrationWidgetMap::~CalibrationWidgetMap()
{

}


Vna *CalibrationWidgetMap::vna() const {
    return _vna;
}
void CalibrationWidgetMap::setVna(Vna *vna) {
    _vna = vna;
    _dialog.setVna(vna);
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
    updateView();

}
QPushButton *CalibrationWidgetMap::pushButton() const {
    return _pushButton;
}
void CalibrationWidgetMap::setPushButton(QPushButton *button) {
    if (_pushButton == button)
        return;

    disconnectView();
    _pushButton = button;
    connectView();
}

bool CalibrationWidgetMap::isCalibration() const {
    return !_calibration.isNull();
}
SharedCalibration CalibrationWidgetMap::calibration() const {
    return _calibration;
}
void CalibrationWidgetMap::setCalibration(SharedCalibration calibration) {
    if (_calibration == calibration)
        return;

    disconnectView();
    _calibration = calibration;
    connectView();
    updateView();
}

void CalibrationWidgetMap::getCalibration() {
    if (!isVna())
        return;

    if (_dialog.exec() == QDialog::Accepted)
        *_calibration = _dialog.calibration();
}
bool CalibrationWidgetMap::isVna() const {
    return _vna != NULL;
}
bool CalibrationWidgetMap::isLineEdit() const {
    return _lineEdit != NULL;
}
bool CalibrationWidgetMap::isPushButton() const {
    return _pushButton != NULL;
}

void CalibrationWidgetMap::connectView() {
    if (isLineEdit() && isCalibration()) {
        QObject::connect(_calibration.data(), SIGNAL(changed(QString)),
                         _lineEdit, SLOT(setText(QString)));
    }
    if (isPushButton()) {
        QObject::connect(_pushButton, SIGNAL(clicked()),
                         this, SLOT(getCalibration()));
    }
}
void CalibrationWidgetMap::disconnectView() {
    if (isLineEdit() && isCalibration()) {
        QObject::disconnect(_calibration.data(), SIGNAL(changed(QString)),
                         _lineEdit, SLOT(setText(QString)));
    }
    if (isPushButton()) {
        QObject::disconnect(_pushButton, SIGNAL(clicked()),
                         this, SLOT(getCalibration()));
    }
}
void CalibrationWidgetMap::updateView() {
    if (isLineEdit())
        _lineEdit->setText(_calibration->displayText());
}

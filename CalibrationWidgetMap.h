#ifndef CalibrationWidgetMap_H
#define CalibrationWidgetMap_H


// Project
#include "Calibration.h"
#include "getCalibrationDialog.h"

// RsaToolbox
#include "Vna.h"

// Qt
#include <QObject>
#include <QLineEdit>
#include <QPushButton>

class CalibrationWidgetMap : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationWidgetMap(QObject *parent = 0);
    ~CalibrationWidgetMap();

    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *edit);

    QPushButton *pushButton() const;
    void setPushButton(QPushButton *button);

    SharedCalibration calibration() const;
    void setCalibration(SharedCalibration calibration);

public slots:
    void getCalibration();

private:
    SharedCalibration _calibration;
    getCalibrationDialog _dialog;

    bool isVna() const;
    RsaToolbox::Vna *_vna;

    bool  isLineEdit() const;
    bool isPushButton() const;
    QLineEdit *_lineEdit;
    QPushButton *_pushButton;
    void connectView();
    void disconnectView();

};

#endif // CalibrationWidgetMap_H

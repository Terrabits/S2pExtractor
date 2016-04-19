#ifndef CalibrationWidgetMap_H
#define CalibrationWidgetMap_H


// Project
#include "Calibration.h"
#include "getCalibrationDialog.h"

// RsaToolbox
#include "Vna.h"
#include "ErrorLabel.h"

// Qt
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class CalibrationWidgetMap : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationWidgetMap(QWidget *parent = 0);
    ~CalibrationWidgetMap();

    bool isCalibration() const;
    Calibration calibration() const;
    void setCalibration(Calibration calibration);

    bool isVna() const;
    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    bool  isLineEdit() const;
    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *edit);

    bool isPushButton() const;
    QPushButton *pushButton() const;
    void setPushButton(QPushButton *button);

public slots:
    void getCalibration();

private:
    Calibration _calibration;
    getCalibrationDialog _dialog;

    RsaToolbox::Vna *_vna;
    QLineEdit *_lineEdit;
    QPushButton *_pushButton;
    void connectView();
    void disconnectView();
    void updateView();
};

#endif // CalibrationWidgetMap_H

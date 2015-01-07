#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
#include "Data.h"
#include "CalibrationWidgetMap.h"
#include "PortsWidgetMap.h"
#include "getFilenamesDialog.h"
#include "CalculateThread.h"

// RsaToolbox
#include "NetworkData.h"

// Qt
#include <QMainWindow>
#include <QScopedPointer>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Data *data, QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_generateButton_clicked();
    void startCalculation();
    void finishCalculation();

    void enableInputs();
    void disableInputs();

    void showPinwheel();
    void hidePinwheel();

private:
    Ui::MainWindow *ui;

    Data *_data;
    CalibrationWidgetMap _outerCalMap;
    CalibrationWidgetMap _innerCalMap;
    PortsWidgetMap _portsMap;
    getFilenamesDialog _filenamesDialog;

    CalculateThread _calcThread;

    bool isReady();

    void initPinwheel();
    QLabel _pinwheel;
};


#endif // MAINWINDOW_H


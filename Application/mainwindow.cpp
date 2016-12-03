#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "Settings.h"
//#include


// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMovie>


MainWindow::MainWindow(Vna *vna, Keys *keys, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(vna),
    _keys(keys),
    _pinwheel(this)

{
    ui->setupUi(this);
    setWindowTitle(APP_NAME + " " + APP_VERSION);
    ui->portOrderCheckbox->setVisible(false);

    ui->outerCal->setVna(_vna);
    ui->innerCal->setVna(_vna);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_generateButton_clicked() {
}

void MainWindow::enableInputs() {
    this->setEnabled(true);
}
void MainWindow::disableInputs() {
    this->setDisabled(true);
}

//void MainWindow::initPinwheel() {
//    _pinwheel.setMovie(new QMovie(":/diagrams/Images/pinwheel.gif"));
//    _pinwheel.resize(32, 32);
//    _pinwheel.raise();
//    _pinwheel.hide();
//}
void MainWindow::showPinwheel() {
    _pinwheel.move(width()/2 - _pinwheel.width()/2,
                   height()/2 - _pinwheel.height()/2);
    _pinwheel.movie()->start();
    _pinwheel.show();
}
void MainWindow::hidePinwheel() {
    _pinwheel.hide();
    _pinwheel.movie()->stop();
}

//bool MainWindow::isReady() {
//    if (_data->outerCalibration().isEmpty()) {
//        ui->error->showMessage("*Choose outer calibration");
//        ui->outerCalButton->setFocus();
//        return false;
//    }
//    if (_data->innerCalibration().isEmpty()) {
//        ui->error->showMessage("*Choose inner calibration");
//        ui->innerCalButton->setFocus();
//        return false;
//    }
//    if (_data->ports().isEmpty()) {
//        ui->error->showMessage("*Choose ports");
//        ui->portsButton->setFocus();
//        return false;
//    }
//    if (!_calcThread.isReady()) {
//        return false;
//    }

//    return true;
//}

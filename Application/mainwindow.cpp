#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "Calculate.h"
#include "commonports.h"
#include "Error.h"
#include "fixfilenameextensions.h"
#include "Settings.h"


// RsaToolbox
#include <Shake.h>
using namespace RsaToolbox;

// Qt
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>

#include <Windows.h>

MainWindow::MainWindow(Vna *vna, Keys *keys, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _vna(vna),
    _keys(keys)
{
    ui->setupUi(this);
    setWindowTitle(APP_NAME + " " + APP_VERSION);
    ui->outerCal->setFocus();

    ui->outerCal->setVna(_vna);
    connect(ui->outerCal, SIGNAL(sourceChanged(CalibrationSource)),
            this, SLOT(updateCals()));
    connect(ui->innerCal, SIGNAL(sourceChanged(CalibrationSource)),
            this, SLOT(updateCals()));
    ui->innerCal->setVna(_vna);

    _thread.reset(new QThread);
    _thread->start();
}
MainWindow::~MainWindow()
{
    delete ui;
    _thread->quit();
    _thread->deleteLater();
    _thread.take();
}

void MainWindow::startGeneration() {
    // TODO: init progress bar
    qDebug() << "start generation";
    setDisabled(true);
}
void MainWindow::showGenerationProgress(int percent) {
    // TODO: update progress bar
    qDebug() << "  percent complete: " << percent;
}
void MainWindow::showGenerationError(QString message) {
    _isError = true;
    showError(message);
}

void MainWindow::endGeneration() {
    qDebug() << "ending generation...";
    if (_isError) {
        // TODO: cleanup progress bar
        setEnabled(true);
        return;
    }

    _save.reset(new SaveResults(_calculate.data(), ui->ports->filenames()));
    _save->moveToThread(_thread.data());
    connect(_save.data(), SIGNAL(starting()),
            this, SLOT(startSave()));
    connect(_save.data(), SIGNAL(processingFile(uint,QString)),
            this, SLOT(showSaveStatus(uint,QString)));
    connect(_save.data(), SIGNAL(error(uint,QString)),
            this, SLOT(showSaveError(uint,QString)));
    connect(_save.data(), SIGNAL(finished()),
            this, SLOT(endSave()));
    QMetaObject::invokeMethod(_save.data(), "run");
}
void MainWindow::startSave() {
    // TODO: init progress bar
    qDebug() << "starting save...";
}
void MainWindow::showSaveStatus(uint port, QString filename) {
    QString msg = "Saving port %1 to \'%2\'";
    msg = msg.arg(port);
    msg = msg.arg(filename);
    // TODO: update progress bar
    qDebug() << msg;
}
void MainWindow::showSaveError(uint port, QString filename) {
    _isError = true;
    QString msg = "*error saving port %1 to \'%2\'";
    msg = msg.arg(port);
    msg = msg.arg(filename);
    showError(msg);
}
void MainWindow::endSave() {
    // TODO: cleanup progress bar
    setEnabled(true);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_generate_clicked() {
    const CalibrationSource outerSource = ui->outerCal->source();
    const CalibrationSource innerSource = ui->innerCal->source();
    const QMap<uint, QString> filenames = ui->ports->filenames();

    _isError = false;
    QList<uint> ports = filenames.keys();
    std::sort(ports.begin(), ports.end());
    _calculate.reset(new Calculate(outerSource, innerSource, ports.toVector(), _vna));
    Error error;
    if (!_calculate->isReady(error)) {
        switch (error.code) {
        case Error::Code::OuterCalibration:
            ui->outerCal->setFocus();
            break;
        case Error::Code::InnerCalibration:
            ui->innerCal->setFocus();
            break;
        case Error::Code::Ports:
            ui->ports->setFocus();
            break;
        case Error::Code::Other:
            ui->outerCal->setFocus();
            break;
        case Error::Code::None:
            break;
        }
        showError(error.message);
        return;
    }
    _calculate->moveToThread(_thread.data());
    _save.reset();
    connect(_calculate.data(), SIGNAL(started()),
            this, SLOT(startGeneration()));
    connect(_calculate.data(), SIGNAL(progress(int)),
            this, SLOT(showGenerationProgress(int)));
    connect(_calculate.data(), SIGNAL(error(QString)),
            this, SLOT(showGenerationError(QString)));
    connect(_calculate.data(), SIGNAL(finished()),
            this, SLOT(endGeneration()));
    QMetaObject::invokeMethod(_calculate.data(), "run");
}
void MainWindow::showError(const QString &message) {
    ui->error->showMessage(message);
    shake(this);
}

void MainWindow::enableInputs() {
    this->setEnabled(true);
}
void MainWindow::disableInputs() {
    this->setDisabled(true);
}

void MainWindow::updateCals() {
    if (ui->outerCal->source().isEmpty()) {
        ui->ports->setDisabled(true);
        return;
    }
    if (ui->innerCal->source().isEmpty()) {
        ui->ports->setDisabled(true);
        return;
    }
    Channel::Error error;
    Channel outerChannel(ui->outerCal->source(), _vna);
    if (!outerChannel.isReady(error)) {
        showError(error.message);
        ui->ports->setDisabled(true);
        ui->outerCal->setFocus();
        return;
    }
    Channel innerChannel(ui->innerCal->source(), _vna);
    if (!innerChannel.isReady(error)) {
        showError(error.message);
        ui->ports->setDisabled(true);
        ui->innerCal->setFocus();
        return;
    }
    QVector<uint> ports = commonPorts(&outerChannel, &innerChannel);
    ui->ports->setPorts(ports);
    ui->ports->setEnabled(true);
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

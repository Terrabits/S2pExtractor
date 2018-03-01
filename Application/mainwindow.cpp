#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "Calculate.h"
#include "commonports.h"
#include "Error.h"
#include "filenames.h"
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

    loadKeys();
}
MainWindow::~MainWindow()
{
    delete ui;
    _thread->quit();
    _thread->deleteLater();
    _thread.take();
}

void MainWindow::startGeneration() {
    qDebug() << "start generation";
    ui->status->errorLabel()->clearMessage();
    showProgress(0, "Calculating");
    setDisabled(true);
}
void MainWindow::showGenerationProgress(int percent) {
    qDebug() << "progress: " << percent << "%";
    // TODO: update progress bar
    showProgress(percent, "Calculating");
}
void MainWindow::showGenerationError(QString message) {
    qDebug() << "error: " << message;
    _isError = true;
    showError(message);
}

void MainWindow::endGeneration() {
    qDebug() << "end generation";
    if (_isError) {
        qDebug() << "quitting due to error...";
        setEnabled(true);
        return;
    }
    QMap<uint, QString> filenames = addPaths(ui->ports->filenames(), ui->directory->path());
    _save.reset(new SaveResults(_calculate.data(), filenames));
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
    qDebug() << "saving...";
    showProgress(0, "Saving...");
}
void MainWindow::showSaveStatus(uint port, QString filename) {
    qDebug() << "filename " << filename;
    Q_UNUSED(port);
    QString msg = "Saving \'%1\'";
    msg         = msg.arg(filename);
    showProgress(-1, msg);
}
void MainWindow::showSaveError(uint port, QString filename) {
    qDebug() << "save error: " << filename;
    _isError = true;
    QString msg = "*error saving port %1 to \'%2\'";
    msg = msg.arg(port);
    msg = msg.arg(filename);
    showError(msg);
}
void MainWindow::endSave() {
    qDebug() << "end save";
    if (!_isError) {
        ui->status->showErrorLabel();
        ui->status->errorLabel()->showMessage("*Extraction complete!", Qt::darkGreen);
    }
    setEnabled(true);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_generate_clicked() {
    const CalibrationSource outerSource = ui->outerCal->source();
    const CalibrationSource innerSource = ui->innerCal->source();
    QString path                        = ui->directory->path();
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
    if (!checkFilePaths()) {
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
    ui->status->showErrorLabel();
    ui->status->errorLabel()->showMessage(message);
    shake(this);
}
void MainWindow::showProgress(int percent, const QString &format) {
    ui->status->showProgressBar();
    QProgressBar *progress = ui->status->progressBar();
    if (percent < 0 || percent > 100) {
        progress->setRange(0,0);
    }
    else {
        progress->setRange(0, 100);
        progress->setValue(percent);
    }
    if (format.trimmed().isEmpty()) {
        progress->setTextVisible(false);
    }
    else {
        progress->setTextVisible(true);
        progress->setFormat(format);
    }
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

void MainWindow::saveKeys() {
    _keys->set("OUTER_SOURCE", ui->outerCal->source());
    _keys->set("INNER_SOURCE", ui->innerCal->source());
    _keys->set("EXPORT_DIRECTORY", ui->directory->path());
    _keys->set("IS_REVERSED_PORTS", ui->reversePorts->isChecked());
}
void MainWindow::loadKeys() {
    if (_keys->exists("OUTER_SOURCE")) {
        CalibrationSource source;
        _keys->get("OUTER_SOURCE", source);
        ui->outerCal->setSource(source);
    }
    if (_keys->exists("INNER_SOURCE")) {
        CalibrationSource source;
        _keys->get("INNER_SOURCE", source);
        ui->innerCal->setSource(source);
    }
    if (_keys->exists("EXPORT_DIRECTORY")) {
        QString path;
        _keys->get("EXPORT_DIRECTORY", path);
        QFileInfo info(path);
        if (info.exists()
                && info.isDir()
                && info.isWritable())
        {
            ui->directory->setPath(path);
        }
    }
}

bool MainWindow::checkFilePaths() {
    QString path = ui->directory->path();
    QFileInfo info(path);
    if (!info.exists()) {
        showError("*save directory does not exist");
        ui->directory->setFocus();
        return false;
    }
    if (!info.isDir()) {
        showError("*save location is not directory");
        ui->directory->setFocus();
        return false;
    }
    if (!info.isWritable()) {
        showError("*save directory is not writeable");
        ui->directory->setFocus();
        return false;
    }

    const QMap<uint, QString> filenames = ui->ports->filenames();
    foreach (const QString filename, filenames.values()) {
        if (!isValidFilename(filename)) {
            QString msg = "*invalid filename: \'%1\'";
            msg = msg.arg(filename);
            showError(msg);
            ui->ports->setFocus();
            return false;
        }
    }
    return true;
}

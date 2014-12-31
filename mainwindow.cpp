#include "QDebug"

// RsaDeembed
#include "Settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

// RsaToolbox
#include "Touchstone.h"
#include "getFilenamesDialog.h"
using namespace RsaToolbox;

// Qt
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>


MainWindow::MainWindow(Vna &vna, RsaToolbox::Keys &keys, QWidget *parent) :
    _vna(vna),
    _keys(keys),
//    _portsDialog(vna, this),
//    _outerCalDialog(vna, this),
//    _innerCalDialog(vna, this),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow constructor";
    ui->setupUi(this);
    QString title = APP_NAME + " " + APP_VERSION;
    setWindowTitle(title);

    qDebug() << "1";
    ui->portsEdit->clear();
    ui->outerCalEdit->clear();
    ui->innerCalEdit->clear();
    // ui->directoryEdit->clear();
    ui->portOrderCheckbox->setVisible(false);

    qDebug() << "2";
    _deleteOuterChannel = false;
    _deleteInnerChannel = false;
    _outerChannel = 0;
    _innerChannel = 0;
    _outerPorts.clear();
    _innerPorts.clear();

    qDebug() << "3";
    _points = 0;
    _ports.clear();
    _start_Hz = 0;
    _stop_Hz = 0;
    _sweepType = VnaChannel::SweepType::Linear;
    _x_Hz.clear();

    qDebug() << "4";
    _outerCalDialog.setDefaultChannel(1);
    _outerCalDialog.selectDefault();
    _updateOuterCal();

    qDebug() << "5";
    _innerCalDialog.setDefaultChannel(2);
    _innerCalDialog.selectDefault();
    _updateInnerCal();

    qDebug() << "6";
    if (_innerCalDialog.isCalibrationSelected()
            && _outerCalDialog.isCalibrationSelected()
            && _commonPorts().isEmpty() == false)
    {
        _portsDialog.setDefault(_commonPorts());
        _portsDialog.selectDefault();
        _updatePorts();
    }
    else {
        _portsDialog.addDefault(1);
        _portsDialog.addDefault(2);
        _innerCalDialog.clearSelection();
        _updateInnerCal();
        _outerCalDialog.clearSelection();
        _updateOuterCal();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_portOrderCheckbox_toggled(bool checked) {
    if (checked)
        ui->diagramLabel->setPixmap(QPixmap(":/diagrams/Graphics/CalPlanes_Reverse_150.png"));
    else
        ui->diagramLabel->setPixmap(QPixmap(":/diagrams/Graphics/CalPlanes_150.png"));
}
void MainWindow::on_outerCalButton_clicked() {
    _outerCalDialog.exec(_vna);
    if (_outerCalDialog.isOkClicked())
        _updateOuterCal();
}
void MainWindow::on_innerCalButton_clicked() {
    _innerCalDialog.exec(_vna);
    if (_innerCalDialog.isOkClicked())
        _updateInnerCal();
}
//void MainWindow::on_directoryButton_clicked() {
//    QString directory;
//    if (_directory.isEmpty())
//        directory = vna.fileSystem().directory(TRACES_DIRECTORY);
//    else
//        directory = _directory;

//    directory = QFileDialog::getExistingDirectory(this, "Chooose dir...", directory);
//    if (directory.isEmpty() == false) {
//        this->_directory = directory;
//        _updateDirectory();
//    }
//}
void MainWindow::on_portsButton_clicked() {
    _ports = _commonPorts();
    if (_ports.isEmpty() == false) {
        _portsDialog.setDefault(_ports);
    }
    else {
        _portsDialog.clearDefault();
        _portsDialog.addDefault(1);
        _portsDialog.addDefault(2);
    }

    _portsDialog.exec(_vna);
    _updatePorts();
}
void MainWindow::on_generateButton_clicked() {
    qDebug() << "MainWindow::generate";
    if (_isReadyToGenerate() && _isMatchingCalibrations()
            && _isCalibratedPorts())
    {
        qDebug() << "  Going for it.";
        _constructX();
        QVector<uint> ports = _portsDialog.selectedPorts();

        QString directory;
        directory = _vna.fileSystem().directory(VnaFileSystem::Directory::TRACES_DIRECTORY);
        getFilenamesDialog filenameDialog(ports, directory, this);
        filenameDialog.exec();
        if (filenameDialog.isOkClicked()) {
            directory = filenameDialog.directory();
            QStringList filenames = filenameDialog.filenames();
            int size = filenames.size();
            for (int i = 0; i < size; i++) {
                qDebug() << i;
                qDebug() << "ports: " << ports[i];
                qDebug() << "file: " << filenames[i];
                NetworkData data = _calculateNetwork(ports[i]);
                QFileInfo file(QDir(directory), filenames[i]);
                Touchstone::Write(data, file.filePath());
            }
            _deleteChannels();
            close();
        }
        else {
            _deleteChannels();
        }
    }
}

void MainWindow::_updatePorts() {
    if (_portsDialog.isPortsSelected()) {
        QVector<uint> ports = _portsDialog.selectedPorts();
        QString result = portString(ports);
        ui->portsEdit->setText(result);
    }
}
void MainWindow::_updateOuterCal() {
    if (_outerCalDialog.isCalibrationSelected()) {
        if (_outerCalDialog.isCalGroup()) {
            ui->outerCalEdit->setText(_outerCalDialog.calGroup());
        }
        else if (_outerCalDialog.isCalibratedChannel()) {
            QString label = "Channel %1";
            label = label.arg(_outerCalDialog.channel());
            ui->outerCalEdit->setText(label);
        }
        _loadOuterCorrections();
        _outerPorts = _outerCorrections.ports();
        _deleteChannels();
    }
    else {
        ui->outerCalEdit->clear();
    }
}
void MainWindow::_updateInnerCal() {
    if (_innerCalDialog.isCalibrationSelected()) {
        if (_innerCalDialog.isCalGroup()) {
            ui->innerCalEdit->setText(_innerCalDialog.calGroup());
        }
        else if (_innerCalDialog.isCalibratedChannel()) {
            QString label = "Channel %1";
            label = label.arg(_innerCalDialog.channel());
            ui->innerCalEdit->setText(label);
        }
        _loadInnerCorrections();
        _innerPorts = _innerCorrections.ports();
        _deleteChannels();
    }
    else {
        ui->innerCalEdit->clear();
    }
}
void MainWindow::_updateDirectory() {
    //    if (_directory.isEmpty() == false) {
    //        QString directory
    //                = QDir::toNativeSeparators(_directory);
    //        ui->directoryEdit->setText(directory);
    //    }
}

bool MainWindow::_isReadyToGenerate() {
    if (_outerCalDialog.isCalibrationSelected() == false) {
        QMessageBox::warning(this, "Missing outer calibration...",
                             "Please select the outer calibration.");
        return(false);
    }
    if (_innerCalDialog.isCalibrationSelected() == false) {
        QMessageBox::warning(this, "Missing inner calibration...",
                             "Please select the inner calibration.");
        return(false);
    }
    if (_portsDialog.isPortsSelected() == false) {
        QMessageBox::warning(this, "Missing ports...",
                             "Please select the ports to use during touchstone file generation.");
        ui->portsButton->setFocus();
        return(false);
    }
    //    if (_directory.isEmpty()) {
    //        QMessageBox::warning(this, "Missing directory...",
    //                             "Please select the directory for saving touchstone files.");
    //        return(false);
    //    }
    return(true);
}
void MainWindow::_loadOuterCorrections() {
    if (_outerCalDialog.isCalibratedChannel()) {
        _deleteOuterChannel = false;
        _outerChannel = _outerCalDialog.channel();
    }
    else {
        _deleteOuterChannel = true;
        _outerChannel = _vna.createChannel();
        QString calGroup = _outerCalDialog.calGroup();
        _vna.channel(_outerChannel).setCalGroup(calGroup);
    }
    _outerCorrections = _vna.channel(_outerChannel).corrections();
}
void MainWindow::_loadInnerCorrections() {
    if (_innerCalDialog.isCalibratedChannel()) {
        _deleteInnerChannel = false;
        _innerChannel = _innerCalDialog.channel();
    }
    else {
        _deleteInnerChannel = true;
        _innerChannel = _vna.createChannel();
        QString calGroup = _innerCalDialog.calGroup();
        _vna.channel(_innerChannel).setCalGroup(calGroup);
    }
    _innerCorrections = _vna.channel(_innerChannel).corrections();
}
QVector<uint> MainWindow::_commonPorts() {
    QVector<uint> ports;
    foreach(uint port, _outerPorts) {
        if (_innerPorts.contains(port))
            ports.append(port);
    }
    return(ports);
}

bool MainWindow::_isMatchingCalibrations() {
    _loadOuterCorrections();
    _loadInnerCorrections();

    _points = _outerCorrections.points();
    _ports = _commonPorts();
    _start_Hz = _outerCorrections.startFrequency_Hz();
    _stop_Hz = _outerCorrections.stopFrequency_Hz();
    _sweepType = _outerCorrections.sweepType();

    if (_innerCorrections.points() != _points) {
        QMessageBox::warning(this, "Calibrations do not match...", QString() +
                             "The inner and outer calibrations must have the same settings.\n"
                             + "Please make sure the number of points in both calibrations match.");
        _deleteChannels();
        return(false);
    }
    if (_ports.isEmpty()) {
        QMessageBox::warning(this, "Calibrated ports do not match...", QString() +
                             "The inner and outer calibrations must use overlapping sets of ports.\n"
                             + "Please make sure at least one port is common to both calibrations.");
        _deleteChannels();
        return(false);
    }
    if (_innerCorrections.startFrequency_Hz() != _start_Hz
            || _innerCorrections.stopFrequency_Hz() != _stop_Hz
            || _innerCorrections.sweepType() != _sweepType) {
        QMessageBox::warning(this, "Calibrations do not match...", QString() +
                             "The inner and outer calibrations must have the same settings.\n"
                             + "Please ensure the frequency points in both calibrations match.");
        _deleteChannels();
        return(false);
    }

    // Else
    return(true);
}
bool MainWindow::_isCalibratedPorts() {
    foreach(uint i, _portsDialog.selectedPorts()) {
        if (_ports.contains(i) == false) {
            QMessageBox::warning(this, "Ports not calibrated...",
                                 QString("Port %1").arg(i) +
                                 " is not included in both calibrations.\n"
                                 + "Please recalibrate or choose different ports.");
            _deleteChannels();
            return(false);
        }
    }
    // Else
    return(true);
}
NetworkData MainWindow::_calculateNetwork(uint port) {
    qDebug() << "MainWindow::_calulateNetwork " << port;
    qDebug() << "_ports: " << _ports;
    uint otherPort;
    if (_ports.first() == port)
        otherPort = _ports.last();
    else
        otherPort = _ports.first();

    ComplexRowVector ed, er, es;
    ed = _outerCorrections.directivity(otherPort, port);
    er = _outerCorrections.reflectionTracking(otherPort, port);
    es = _outerCorrections.sourceMatch(otherPort, port);

    ComplexRowVector edFix, erFix, esFix;
    edFix = _innerCorrections.directivity(otherPort, port);
    erFix = _innerCorrections.reflectionTracking(otherPort, port);
    esFix = _innerCorrections.sourceMatch(otherPort, port);

    ComplexRowVector denominator = add(er, multiplyEach(es, subtract(edFix, ed)));
    ComplexRowVector s11 = divideEach(subtract(edFix, ed), denominator);
    ComplexRowVector s22 = subtract(esFix, divideEach(multiplyEach(es, erFix), denominator));

    ComplexRowVector s21Numerator = smoothSqrt(multiplyEach(er, erFix));
    ComplexRowVector s21 = divideEach(s21Numerator, denominator);
    s21 = smoothPhase(s21);
    s21 = fixPhaseAtDc(_x_Hz, s21);

    ComplexMatrix3D y;
    _constructMatrix(y, s11, s21, s22);

    NetworkData data;
    data.setComment("");
    data.setParameter(NetworkParameter::S);
    data.setXUnits(Units::Hertz);
    data.setData(_x_Hz, y);
    return(data);
}
void MainWindow::_constructX() {
    if (_sweepType == VnaChannel::SweepType::Linear)
        linearSpacing(_x_Hz, _start_Hz, _stop_Hz, _points);
    else if (_outerCorrections.sweepType() == VnaChannel::SweepType::Log)
        logSpacing(_x_Hz, _start_Hz, _stop_Hz, _points);
    else
        // Cannot calculate frequency points!
        _x_Hz.clear();
}
void MainWindow::_constructMatrix(ComplexMatrix3D &matrix, ComplexRowVector &s11, ComplexRowVector &s21, ComplexRowVector &s22) {
    qDebug() << "points: " << _points;
    matrix.resize(_points);
    for (uint i = 0; i < _points; i++) {
        matrix[i].resize(2);
        matrix[i][0].resize(2);
        matrix[i][1].resize(2);
        matrix[i][0][0] = s11[i];
        matrix[i][0][1] = s21[i];
        matrix[i][1][0] = s21[i];
        matrix[i][1][1] = s22[i];
    }
}
void MainWindow::_deleteChannels() {
    if (_deleteOuterChannel) {
        _deleteOuterChannel = false;
        _vna.deleteChannel(_outerChannel);
    }
    if (_deleteInnerChannel) {
        _deleteInnerChannel = false;
        _vna.deleteChannel(_innerChannel);
    }
}


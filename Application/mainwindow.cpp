#include "mainwindow.h"
#include "ui_mainwindow.h"


// Project
#include "Settings.h"
#include "Calibration.h"


// RsaToolbox
#include "Touchstone.h"
#include "getFilenamesDialog.h"
using namespace RsaToolbox;

// Qt
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMovie>


MainWindow::MainWindow(Data *data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)/*,*/
//    _data(data),
//    _outerCalMap(this),
//    _innerCalMap(this),
//    _portsMap(this),
//    _filenamesDialog(this),
//    _pinwheel(this)
{
    ui->setupUi(this);
    setWindowTitle(APP_NAME + " " + APP_VERSION);
    ui->portOrderCheckbox->setVisible(false);

//    _outerCalMap.setVna(_data->vna());
//    _outerCalMap.setLineEdit(ui->outerCalEdit);
//    _outerCalMap.setPushButton(ui->outerCalButton);
//    _outerCalMap.setCalibration(_data->outerCalibration());

//    _innerCalMap.setVna(_data->vna());
//    _innerCalMap.setLineEdit(ui->innerCalEdit);
//    _innerCalMap.setPushButton(ui->innerCalButton);
//    _innerCalMap.setCalibration(_data->innerCalibration());

//    _portsMap.setVna(_data->vna());
//    _portsMap.setLineEdit(ui->portsEdit);
//    _portsMap.setPushButton(ui->portsButton);
//    _portsMap.setPorts(_data->ports());

//    _filenamesDialog.setPorts(_data->ports());
//    _filenamesDialog.setKey(_data->keys(), SAVE_PATH_KEY);

////    _calcThread.setData(_data);

//    initPinwheel();

//    QObject::connect(&_calcThread, SIGNAL(error(QString)),
//                     ui->error, SLOT(showMessage(QString)));

//    QObject::connect(&_calcThread, SIGNAL(started()),
//                     this, SLOT(disableInputs()));
//    QObject::connect(&_calcThread, SIGNAL(started()),
//                     this, SLOT(showPinwheel()));

//    QObject::connect(&_calcThread, SIGNAL(finished()),
//                     this, SLOT(hidePinwheel()));
//    QObject::connect(&_calcThread, SIGNAL(finished()),
//                     this, SLOT(enableInputs()));
//    QObject::connect(&_calcThread, SIGNAL(finished()),
//                     this, SLOT(finishCalculation()));

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
//    const int key = event->key();
//    if (key == Qt::Key_Escape) {
//        event->accept();
//        close();
//        return;
//    }

//    if (key == Qt::Key_Enter || key == Qt::Key_Return) {
//        QWidget *widget = this->focusWidget();
//        if (widget == ui->portOrderCheckbox) {
//            ui->portOrderCheckbox->click();
//            event->accept();
//            return;
//        }
//        else if (widget == ui->outerCalButton) {
//            ui->outerCalButton->click();
//            event->accept();
//            return;
//        }
//        else if (widget == ui->innerCalButton) {
//            ui->innerCalButton->click();
//            event->accept();
//            return;
//        }
//        else if (widget == ui->portsButton) {
//            ui->portsButton->click();
//            event->accept();
//            return;
//        }
//        else if (widget == ui->generateButton) {
//            ui->generateButton->click();
//            event->accept();
//            return;
//        }
//    }

//    // Else
//    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_generateButton_clicked() {
//    if (!isReady())
//        return;
//    if (_filenamesDialog.exec() != QDialog::Accepted)
//        return;

//    _data->filenames() = _filenamesDialog.filePathNames();
//    startCalculation();
}
void MainWindow::startCalculation() {
//    disableInputs();
//    showPinwheel();
//    _calcThread.start();
}
void MainWindow::finishCalculation() {
//    hidePinwheel();
//    enableInputs();

//    if (_calcThread.isError())
//        return;

//    QMessageBox::information(this,
//                             APP_NAME,
//                             "Touchstone files successfully generated!");
//    close();
}

void MainWindow::enableInputs() {
//    this->setEnabled(true);
}
void MainWindow::disableInputs() {
//    this->setDisabled(true);
}

//void MainWindow::initPinwheel() {
//    _pinwheel.setMovie(new QMovie(":/diagrams/Images/pinwheel.gif"));
//    _pinwheel.resize(32, 32);
//    _pinwheel.raise();
//    _pinwheel.hide();
//}
void MainWindow::showPinwheel() {
//    _pinwheel.move(width()/2 - _pinwheel.width()/2,
//                   height()/2 - _pinwheel.height()/2);
//    _pinwheel.movie()->start();
//    _pinwheel.show();
}
void MainWindow::hidePinwheel() {
//    _pinwheel.hide();
//    _pinwheel.movie()->stop();
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

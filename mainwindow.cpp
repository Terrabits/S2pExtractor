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
#include <QKeyEvent>


MainWindow::MainWindow(Vna &vna, RsaToolbox::Keys &keys, QWidget *parent) :
    _vna(vna),
    _keys(keys),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(APP_NAME + " " + APP_VERSION);
    ui->portOrderCheckbox->setVisible(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    const int key = event->key();
    if (key == Qt::Key_Escape) {
        event->accept();
        close();
        return;
    }

    if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        QWidget *widget = this->focusWidget();
        if (widget == ui->portOrderCheckbox) {
            ui->portOrderCheckbox->click();
            event->accept();
            return;
        }
        else if (widget == ui->outerCalButton) {
            ui->outerCalButton->click();
            event->accept();
            return;
        }
        else if (widget == ui->innerCalButton) {
            ui->innerCalButton->click();
            event->accept();
            return;
        }
        else if (widget == ui->portsButton) {
            ui->portsButton->click();
            event->accept();
            return;
        }
        else if (widget == ui->generateButton) {
            ui->generateButton->click();
            event->accept();
            return;
        }
    }

    // Else
    QMainWindow::keyPressEvent(event);
}


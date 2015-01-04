#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// RsaToolbox
#include "Keys.h"
#include "NetworkData.h"
#include "getPortsDialog.h"
#include "getCalibrationDialog.h"
#include "Vna.h"

// Qt
#include <QMainWindow>
#include <QScopedPointer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(RsaToolbox::Vna &_vna, RsaToolbox::Keys &_keys, QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:

private:
    Ui::MainWindow *ui;
    RsaToolbox::Vna &_vna;
    RsaToolbox::Keys &_keys;
};


#endif // MAINWINDOW_H


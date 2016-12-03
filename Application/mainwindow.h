#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
//#include

// RsaToolbox
#include <Keys.h>
#include <Vna.h>

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
    explicit MainWindow(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys, QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_generateButton_clicked();

    void enableInputs();
    void disableInputs();

    void showPinwheel();
    void hidePinwheel();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna  *_vna;
    RsaToolbox::Keys *_keys;

    void initPinwheel();
    QLabel _pinwheel;
};


#endif // MAINWINDOW_H


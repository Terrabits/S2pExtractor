#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
//#include

// RsaToolbox
//#include

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
    explicit MainWindow(QWidget *parent = 0);
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

    void initPinwheel();
    QLabel _pinwheel;
};


#endif // MAINWINDOW_H


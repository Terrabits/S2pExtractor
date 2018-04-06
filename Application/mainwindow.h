#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Project
#include "Calculate.h"
#include "CalibrationSource.h"
#include "Channel.h"
#include "saveresults.h"

// RsaToolbox
#include <Keys.h>
#include <Vna.h>

// Qt
#include <QLabel>
#include <QMainWindow>
#include <QScopedPointer>
#include <QThread>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(RsaToolbox::Vna *vna, RsaToolbox::Keys *keys, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startGeneration();
    void showGenerationProgress(int percent);
    void showGenerationError(QString message);
    void endGeneration();
    void startSave();
    void showSaveStatus(uint port, QString filename);
    void showSaveError(uint port, QString filename);
    void endSave();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_generate_clicked();

    void showError(const QString &message);
    void showProgress(int percent, const QString &format = "%p%");
    void enableInputs();
    void disableInputs();
    void updateCals();

    void on_close_clicked();

private:
    Ui::MainWindow *ui;

    RsaToolbox::Vna  *_vna;
    RsaToolbox::Keys *_keys;
    void saveKeys();
    void loadKeys();
    bool checkFilePaths();

    bool _isError;
    QScopedPointer<Calculate>   _calculate;
    QScopedPointer<SaveResults> _save;
    QScopedPointer<QThread>     _thread;
};


#endif // MAINWINDOW_H


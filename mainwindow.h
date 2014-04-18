#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// RsaToolbox
#include "Key.h"
#include "Vna.h"
#include "NetworkData.h"
#include "getPortsDialog.h"
#include "getCalibrationDialog.h"

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
    explicit MainWindow(RsaToolbox::Vna &vna, RsaToolbox::Key &key, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_portOrderCheckbox_toggled(bool checked);
    void on_outerCalButton_clicked();
    void on_innerCalButton_clicked();
    //void on_directoryButton_clicked();
    void on_portsButton_clicked();
    void on_generateButton_clicked();

private:
    Ui::MainWindow *ui;
    RsaToolbox::Vna &vna;
    RsaToolbox::Key &key;

    getPortsDialog _portsDialog;
    getCalibrationDialog _outerCalDialog;
    getCalibrationDialog _innerCalDialog;
    QString _directory;

    void _updatePorts();
    void _updateOuterCal();
    void _updateInnerCal();
    void _updateDirectory();

    bool _deleteOuterChannel;
    uint _outerChannel;
    QVector<uint> _outerPorts;
    RsaToolbox::VnaCorrections _outerCorrections;
    void _loadOuterCorrections();

    bool _deleteInnerChannel;
    uint _innerChannel;
    QVector<uint> _innerPorts;
    RsaToolbox::VnaCorrections _innerCorrections;
    void _loadInnerCorrections();

    QVector<uint> _commonPorts();

    uint _points;
    QVector<uint> _ports;
    double _start_Hz;
    double _stop_Hz;
    RsaToolbox::VnaSweepType _sweepType;
    RsaToolbox::QRowVector _x_Hz;

    bool _isReadyToGenerate();
    bool _isMatchingCalibrations();
    bool _isCalibratedPorts();
    RsaToolbox::NetworkData _calculateNetwork(uint port);
    void _constructX();
    void _constructMatrix(RsaToolbox::ComplexMatrix3D &matrix, RsaToolbox::ComplexRowVector &s11, RsaToolbox::ComplexRowVector &s21, RsaToolbox::ComplexRowVector &s22);
    void _deleteChannels();
};


#endif // MAINWINDOW_H


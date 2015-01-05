#include "getCalibrationDialog.h"
#include "ui_getCalibrationDialog.h"


// RsaToolbox
#include "General.h"
using namespace RsaToolbox;

// Qt
#include <QKeyEvent>
#include <QDebug>


getCalibrationDialog::getCalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getCalibrationDialog)
{
    ui->setupUi(this);

    _vna = NULL;
    reset();

    QObject::connect(&_selection, SIGNAL(error(QString)),
                     ui->error, SLOT(showMessage(QString)));
}
getCalibrationDialog::~getCalibrationDialog() {
    delete ui;
}

Vna *getCalibrationDialog::vna() const {
    return _vna;
}
void getCalibrationDialog::setVna(Vna *vna) {
    _vna = vna;
}

void getCalibrationDialog::reset() {
    _default.clear();
    _selection.clear();
    _calibration.clear();
    updateUi();
    unselectAll();
}
void getCalibrationDialog::unselectAll() {
    ui->channels->clearSelection();
    ui->calGroups->clearSelection();
}
void getCalibrationDialog::select(const Calibration &cal) {
    if (cal.isEmpty())
        return;
    if (cal.isCalGroup()) {
        int row = _calGroups.indexOf(cal.calGroup());
        if (row != -1)
            ui->calGroups->setCurrentRow(row);
    }
    else {
        int row = _channels.indexOf(cal.channel());
        if (row != -1)
            ui->channels->setCurrentRow(row);
    }
}

void getCalibrationDialog::clearDefault() {
    _default.clear();
}
bool getCalibrationDialog::isDefault() const {
    return !_default.isEmpty();
}
Calibration getCalibrationDialog::defaultCalibration() const {
    return _default;
}
void getCalibrationDialog::setDefault(Calibration cal) {
    _default = cal;
}

Calibration getCalibrationDialog::calibration() const {
    return _calibration;
}

int getCalibrationDialog::exec() {
    _selection.clear();
    updateUi();
    unselectAll();
    if (!_calibration.isEmpty())
        select(_calibration);
    else if (!_default.isEmpty())
        select(_default);
    
    int x = parentWidget()->geometry().x();
    x += parentWidget()->width()/2.0;
    x -= width()/2.0;
    
    int y = parentWidget()->geometry().y();
    y += parentWidget()->height()/2.0;
    y -= height()/2.0;

    this->setGeometry(x, y, width(), height());
    return QDialog::exec();
}

void getCalibrationDialog::accept() {
    if (_selection.isEmpty()) {
        ui->error->showMessage("*Please choose a calibration.");
        return;
    }
    if (!_selection.isValid(_vna))
        return;

    _calibration = _selection;
    QDialog::accept();
    emit changed(_calibration);
}
void getCalibrationDialog::reject() {
    _selection.clear();
    QDialog::reject();
}

void getCalibrationDialog::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Escape) {
        event->accept();
        reject();
    }
    else {
        QDialog::keyPressEvent(event);
    }
}


void getCalibrationDialog::on_channels_itemSelectionChanged() {
    const QList<QListWidgetItem*> selectedItems = ui->channels->selectedItems();
    if (selectedItems.isEmpty())
        return;

    ui->calGroups->clearSelection();
    const QListWidgetItem *selectedItem = selectedItems.first();
    const uint row = ui->channels->row(selectedItem);
    _selection.setChannel(_channels[row]);
}
void getCalibrationDialog::on_calGroups_itemSelectionChanged() {
    const QList<QListWidgetItem*> selectedItems = ui->calGroups->selectedItems();
    if (selectedItems.isEmpty())
        return;

    ui->channels->clearSelection();
    const QListWidgetItem *selectedItem = selectedItems.first();
    const uint row = ui->calGroups->row(selectedItem);
    _selection.setCalGroup(_calGroups[row]);
}

bool getCalibrationDialog::isVna() const {
    return _vna != NULL;
}
void getCalibrationDialog::updateUi() {
    if (!isVna())
        return;

    _calGroups = _vna->calGroups();
    ui->calGroups->clear();
    if (!_calGroups.isEmpty())
        ui->calGroups->addItems(_calGroups);

    _channels = _vna->calibratedChannels();
    ui->channels->clear();
    if (!_channels.isEmpty())
        ui->channels->addItems(toStringList(_channels));
}

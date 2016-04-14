#include "getPortsDialog.h"
#include "ui_getPortsDialog.h"


//RsaToolbox
#include "General.h"
using namespace RsaToolbox;

// Qt
#include <QKeyEvent>
#include <QDebug>


getPortsDialog::getPortsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getPortsDialog)
{
    ui->setupUi(this);

    _vna = NULL;
    reset();
}
getPortsDialog::~getPortsDialog()
{
    delete ui;
}

bool getPortsDialog::isVna() const {
    return _vna != NULL;
}
Vna *getPortsDialog::vna() const {
    return _vna;
}
void getPortsDialog::setVna(Vna *vna) {
    _vna = vna;
}

bool getPortsDialog::isDefault() const {
    return !_default.isEmpty();
}
QVector<uint> getPortsDialog::defaultPorts() const {
    return _default;
}
void getPortsDialog::setDefaultPorts(QVector<uint> ports) {
    _default = ports;
}

QVector<uint> getPortsDialog::ports() const {
    return _ports;
}

int getPortsDialog::exec() {
    updateUi();
    uncheckAll();
    if (!_ports.isEmpty())
        check(_ports);
    else if (!_default.isEmpty())
        check(_default);

    int x = parentWidget()->geometry().x();
    x += parentWidget()->width()/2.0;
    x -= width()/2.0;

    int y = parentWidget()->geometry().y();
    y += parentWidget()->height()/2.0;
    y -= height()/2.0;

    this->setGeometry(x, y, width(), height());
    return QDialog::exec();
}
void getPortsDialog::accept() {
    const QVector<uint> _selection = selection();
    if (_selection.isEmpty()) {
        ui->error->showMessage("*Please choose at least one port.");
        return;
    }

    if (_selection == _ports) {
        QDialog::accept();
        return;
    }

    _ports = _selection;
    QDialog::accept();
    emit changed(_ports);
}

void getPortsDialog::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Escape) {
        event->accept();
        reject();
    }
    else {
        QDialog::keyPressEvent(event);
    }
}

void getPortsDialog::on_selectAll_toggled(bool checked) {
    if (checked)
        checkAll();
    else
        uncheckAll();
}

void getPortsDialog::updateUi() {
    if (!isVna())
        return;

    ui->ports->clear();
    _vnaPorts = range(uint(1), _vna->testPorts());
    ui->ports->addItems(portLabels(_vnaPorts));
}

QVector<uint> getPortsDialog::selection() {
    QVector<uint> _selection;
    for (int i = 0; i < ui->ports->count(); i++) {
        if (ui->ports->item(i)->checkState() == Qt::Checked)
            _selection << i+1;
    }
    return _selection;
}

void getPortsDialog::reset() {
    _default.clear();
    _ports.clear();
    uncheckAll();
}
void getPortsDialog::checkAll() {
    for (int i = 0; i < ui->ports->count(); i++) {
        ui->ports->item(i)->setCheckState(Qt::Checked);
    }
}
void getPortsDialog::uncheckAll() {
    for (int i = 0; i < ui->ports->count(); i++) {
        ui->ports->item(i)->setCheckState(Qt::Unchecked);
    }
}
void getPortsDialog::check(const QVector<uint> &ports) {
    foreach (uint port, ports) {
        int row = _vnaPorts.indexOf(port);
        if (row != -1)
            ui->ports->item(row)->setCheckState(Qt::Checked);
    }
}



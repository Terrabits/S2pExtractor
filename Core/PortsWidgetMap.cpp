#include "PortsWidgetMap.h"


// RsaToolbox
#include "General.h"
using namespace RsaToolbox;

// Qt
#include <QDebug>


PortsWidgetMap::PortsWidgetMap(QWidget *parent) :
    _dialog(parent),
    QObject(parent)
{
    _vna = NULL;
    _lineEdit = NULL;
    _pushButton = NULL;
}
PortsWidgetMap::~PortsWidgetMap()
{}

bool PortsWidgetMap::isPorts() const {
    return !_ports.isEmpty();
}
QVector<uint> PortsWidgetMap::ports() const {
    return _ports;
}
void PortsWidgetMap::setPorts(QVector<uint> ports) {
    if (_ports == ports)
        return;

    disconnectView();
    _ports = ports;
    connectView();
    updateView();
}

bool PortsWidgetMap::isVna() const {
    return _vna != NULL;
}
Vna *PortsWidgetMap::vna() const {
    return _vna;
}
void PortsWidgetMap::setVna(Vna *vna) {
    _vna = vna;
    _dialog.setVna(_vna);
}

bool PortsWidgetMap::isLineEdit() const {
    return _lineEdit != NULL;
}
QLineEdit *PortsWidgetMap::lineEdit() const {
    return _lineEdit;
}
void PortsWidgetMap::setLineEdit(QLineEdit *edit) {
    if (_lineEdit == edit)
        return;

    disconnectView();
    _lineEdit = edit;
    connectView();
    updateView();
}

bool PortsWidgetMap::isPushButton() const {
    return _pushButton != NULL;
}
QPushButton *PortsWidgetMap::pushButton() const {
    return _pushButton;
}
void PortsWidgetMap::setPushButton(QPushButton *button) {
    if (_pushButton == button)
        return;

    disconnectView();
    _pushButton = button;
    connectView();
}

void PortsWidgetMap::getPorts() {
    if (!isVna())
        return;

    if (_dialog.exec() == QDialog::Accepted) {
        _ports = _dialog.ports();
        updateView();
    }
}

void PortsWidgetMap::connectView() {
    if (isPushButton()) {
        QObject::connect(_pushButton, SIGNAL(clicked()),
                         this, SLOT(getPorts()));
    }
}
void PortsWidgetMap::disconnectView() {
    if (isPushButton()) {
        QObject::disconnect(_pushButton, SIGNAL(clicked()),
                            this, SLOT(getPorts()));
    }
}
void PortsWidgetMap::updateView() {
    if (isLineEdit())
        _lineEdit->setText(portString(_ports));
}

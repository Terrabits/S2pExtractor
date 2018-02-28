#include "portswidget.h"
#include "ui_portswidget.h"

#include "filenamedelegate.h"
#include "fixfilenameextensions.h"

#include <QDebug>
#include <QKeyEvent>
#include <QList>
#include <QScopedPointer>
#include <QTableWidgetItem>

#include <algorithm>
#include <cassert>

PortsWidget::PortsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortsWidget)
{
    ui->setupUi(this);
    setFocusProxy(ui->table);
    _delegate.reset(new FilenameDelegate);
    ui->table->setItemDelegate(_delegate.data());
}

PortsWidget::~PortsWidget()
{
    delete ui;
}

void PortsWidget::setPorts(const QVector<uint> &ports) {
    foreach (const uint port, ports) {
        if (!_filenames.contains(port)) {
            _filenames[port] = QString();
        }
    }
    foreach (const uint port, _filenames.keys()) {
        if (!ports.contains(port)) {
            _filenames.remove(port);
        }
    }
    applyHistory();
    updateTable();
}
void PortsWidget::setFilenames(const QMap<uint, QString> &filenames) {
    _filenames = filenames;
    fixFilenameExtensions(_filenames);
    updateHistory();
    updateTable();
}
QMap<uint, QString> PortsWidget::filenames() const {
    QMap<uint, QString> results;
    foreach (const uint port, _filenames.keys()) {
        if (!_filenames[port].isEmpty()) {
            results[port] = _filenames[port];
        }
    }
    return results;
}

void PortsWidget::keyPressEvent(QKeyEvent *event) {
    const int key           = event->key();
    const QModelIndex index = ui->table->currentIndex();
    switch (key) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        event->accept();
        ui->table->edit(index);
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        event->accept();
        ui->table->currentItem()->setText("");
        ui->table->edit(index);
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void PortsWidget::on_table_cellChanged(int row, int column)
{
    QTableWidgetItem *item = ui->table->item(row, column);
    QString text = item->text().trimmed();
    if (!text.isEmpty()) {
        fixFilenameExtension(text);
    }
    const uint port  = portAtRow(row);
    _filenames[port] = text; // updateFilenames
    _history[port]   = text; // updateHistory

    const bool blocked = ui->table->blockSignals(true);
    item->setText(text);
    setRowColor(row);
    ui->table->blockSignals(blocked);
}

void PortsWidget::resizeTable() {
    const bool blocked = ui->table->blockSignals(true);
    int currentRows = ui->table->rowCount();
    const int rows        = _filenames.size();
    while (currentRows > rows) {
        ui->table->removeRow(currentRows-1);
        currentRows--;
    }
    while (rows > currentRows) {
        ui->table->insertRow(currentRows);
        QScopedPointer<QTableWidgetItem> item(new QTableWidgetItem);
        // TODO
        ui->table->setItem(currentRows, 0, item.take());

        QScopedPointer<QTableWidgetItem> header(new QTableWidgetItem);
        // TODO
        ui->table->setVerticalHeaderItem(currentRows, header.take());

        currentRows++;
    }
    ui->table->blockSignals(blocked);
}

void PortsWidget::setRowColor(const int row) {
    QTableWidgetItem *item   = ui->table->item(row, 0);
    QTableWidgetItem *header = ui->table->verticalHeaderItem(row);
    const bool blocked       = ui->table->blockSignals(true);
    if (!item->text().isEmpty()) {
        header->setTextColor(Qt::black);
        item  ->setTextColor(Qt::black);
    }
    else {
        header->setTextColor(Qt::lightGray);
        item  ->setTextColor(Qt::lightGray);
    }
    ui->table->blockSignals(blocked);
}

void PortsWidget::updateTable() {
    const bool blocked = ui->table->blockSignals(true);
    resizeTable();
    const QList<uint> _ports = ports();
    foreach (const uint port, _ports) {
        const uint row = rowForPort(port);
        QTableWidgetItem *item = ui->table->item(row, 0);
        QTableWidgetItem *header = ui->table->verticalHeaderItem(row);
        item->setText(_filenames[port]);
        header->setText(QString("Port %1").arg(port));
        setRowColor(row);
    }
    if (!ui->table->currentIndex().isValid()) {
       ui->table->setCurrentCell(0,0);
    }
    if (ui->table->hasFocus()) {
        ui->table->selectRow(ui->table->currentRow());
    }
    else {
        ui->table->clearSelection();
    }
    ui->table->blockSignals(blocked);
}

QList<uint> PortsWidget::ports() const {
    QList<uint> _ports = _filenames.keys();
    std::sort(_ports.begin(), _ports.end());
    return _ports;
}
uint PortsWidget::portAtRow(const int row) const {
    return ports()[row];
}
int PortsWidget::rowForPort(const uint port) const {
    return ports().indexOf(port);
}
void PortsWidget::updateHistory() {
    const QMap<uint, QString> _filenames = filenames();
    foreach (const uint port, _filenames.keys()) {
        _history[port] = _filenames[port];
    }
}
void PortsWidget::applyHistory() {
    foreach (const uint port, ports()) {
        if (_history.contains(port)) {
            _filenames[port] = _history[port];
        }
    }
}

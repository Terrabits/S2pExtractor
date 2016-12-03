#include "CalDialog.h"
#include "ui_CalDialog.h"


// RsaToolbox
#include <General.h>
#include <Shake.h>
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalDialog::CalDialog(const QStringList &calGroups, const QVector<uint> &channels, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getCalDialog)
{
    ui->setupUi(this);
    setCalGroupsList(calGroups);
    setChannelsList(channels);
}

CalDialog::~CalDialog()
{
    delete ui;
}

CalibrationSource CalDialog::source() const {
    return _source;
}
void CalDialog::setSource(CalibrationSource source) {
    _source = source;
    selectSource();
}

void CalDialog::accept() {
    _source = selectedSource();
    if (_source.isEmpty()) {
        ui->error->showMessage("*select calibration");
        shake(this);
        return;
    }

    QDialog::accept();
}
void CalDialog::reject() {
    _source.clear();
    clearSelection();
    QDialog::reject();
}

void CalDialog::setCalGroupsList(const QStringList &groups) {
    _calGroups = groups;

    // chop off .cal
    for (int i = 0; i < _calGroups.size(); i++) {
        QString cal = _calGroups[i];
        if (cal.endsWith(".cal", Qt::CaseInsensitive)) {
            cal.chop(4);
            _calGroups[i] = cal;
        }
    }

    updateCalGroups();
}
void CalDialog::setChannelsList(const QVector<uint> &channels) {
    _channels = channels;
    updateChannels();
}
bool CalDialog::isCalGroupTab() const {
    return ui->tabWidget->indexOf(ui->calGroupsTab) != -1;
}
void CalDialog::addCalGroupTab() {
    if (!isCalGroupTab())
        ui->tabWidget->insertTab(0, ui->calGroupsTab, "Cal Group");
}
void CalDialog::removeCalGroupTab() {
    if (isCalGroupTab())
        ui->tabWidget->removeTab(0);
}

bool CalDialog::isChannelTab() const {
    return ui->tabWidget->indexOf(ui->channelsTab) != -1;
}
void CalDialog::addChannelTab() {
    if (!isChannelTab())
        ui->tabWidget->addTab(ui->channelsTab, "Channel");
}
void CalDialog::removeChannelTab() {
    if (isChannelTab())
        ui->tabWidget->removeTab(ui->tabWidget->count()-1);
}

void CalDialog::updateCalGroups() {
    ui->calGroups->clear();
    if (!_calGroups.isEmpty()) {
        addCalGroupTab();
        ui->calGroups->addItems(_calGroups);
        if (_source.isCalGroup())
            selectSource();
    }
    else {
        removeCalGroupTab();
    }
}
void CalDialog::updateChannels() {
    ui->channels->clear();
    if (!_channels.isEmpty()) {
        addChannelTab();
        ui->channels->addItems(toStringList(_channels));
        if (_source.isChannel())
            selectSource();
    }
    else {
        removeChannelTab();
    }
}

CalibrationSource CalDialog::selectedSource() {
    if (ui->tabWidget->currentWidget() == ui->calGroupsTab) {
        if (!ui->calGroups->selectedItems().isEmpty()) {
            const int i = ui->calGroups->currentRow();
            return CalibrationSource(_calGroups[i]);
        }
        else {
            return CalibrationSource();
        }
    }
    else {
        if (!ui->channels->selectedItems().isEmpty()) {
            const int i = ui->channels->currentRow();
            return CalibrationSource(_channels[i]);
        }
        else {
            return CalibrationSource();
        }
    }
}
void CalDialog::clearSelection() {
    ui->calGroups->clearSelection();
    ui->channels->clearSelection();
}
void CalDialog::selectSource() {
    clearSelection();
    if (_source.isCalGroup()) {
        const QString selection = _source.calGroup();
        for (int i = 0; i < _calGroups.size(); i++) {
            const QString cal = _calGroups[i];
            if (cal.compare(selection, Qt::CaseInsensitive) == 0) {
                ui->tabWidget->setCurrentWidget(ui->calGroupsTab);
                ui->calGroups->setFocus();
                ui->calGroups->setCurrentRow(i);
                return;
            }
        }
    }
    if (_source.isChannel()) {
        const uint selection = _source.channel();
        for (int i = 0; i < _channels.size(); i++) {
            const uint ch = _channels[i];
            if (ch == selection) {
                ui->tabWidget->setCurrentWidget(ui->channelsTab);
                ui->channels->setFocus();
                ui->channels->setCurrentRow(i);
                return;
            }
        }
    }
}

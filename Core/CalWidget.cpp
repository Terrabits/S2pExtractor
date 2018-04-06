#include "CalWidget.h"


// Project
#include "CalDialog.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalWidget::CalWidget(QWidget *parent) :
    LabeledButton(parent),
    _vna(0)
{
    connect(this, SIGNAL(buttonClicked()),
            this, SLOT(showDialog()));
}

CalWidget::~CalWidget()
{
}

void CalWidget::setVna(Vna *vna) {
    _vna = vna;
}

void CalWidget::setDialogTitle(const QString &title) {
    _dialogTitle = title;
}

CalibrationSource CalWidget::source() const {
    return _source;
}
void CalWidget::setSource(CalibrationSource source) {
    if (_source == source)
        return;

    _source = source;
    updateLabel();
    emit sourceChanged(_source);
}

void CalWidget::showDialog() {
    CalDialog dialog(calGroups(), calibratedChannels(), topLevelWidget());
    dialog.setWindowTitle(_dialogTitle);
    dialog.setSource(_source);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        setSource(dialog.source());
    }
}

void CalWidget::updateLabel() {
    if (!_source.isEmpty()) {
        setLabel(_source.displayText());
    }
    else {
        clearLabel();
    }
}

bool CalWidget::isVna() const {
    return _vna != 0;
}

QStringList CalWidget::calGroups() {
    return _vna->calGroups();
}
QVector<uint> CalWidget::calibratedChannels() {
    QVector<uint> result;
    foreach (uint c, _vna->channels()) {
        if (_vna->channel(c).isCalibrated())
            result << c;
    }
    return result;
}

#include "CalWidget.h"
#include "ui_CalWidget.h"


// Project
#include "CalDialog.h"

// RsaToolbox
using namespace RsaToolbox;

// Qt
#include <QDebug>


CalWidget::CalWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalWidget),
    _vna(0)
{
    ui->setupUi(this);
}

CalWidget::~CalWidget()
{
    delete ui;
}

void CalWidget::setVna(Vna *vna) {
    _vna = vna;
}

CalibrationSource CalWidget::source() const {
    return _source;
}
void CalWidget::setSource(CalibrationSource source) {
    _source = source;
    updateDisplay();
}

void CalWidget::on_pushButton_clicked() {
    CalDialog dialog(calGroups(), channels(), topLevelWidget());
    dialog.setSource(_source);
    dialog.exec();

    if (dialog.result() == QDialog::Accepted) {
        _source = dialog.source();
        updateDisplay();
    }
}

void CalWidget::updateDisplay() {
    if (!_source.isEmpty()) {
        ui->lineEdit->setText(_source.displayText());
    }
    else {
        ui->lineEdit->clear();
    }
}

bool CalWidget::isVna() const {
    return _vna != 0;
}

QStringList CalWidget::calGroups() {
    return _vna->calGroups();
}
QVector<uint> CalWidget::channels() {
    QVector<uint> result;
    foreach (uint c, _vna->channels()) {
        if (_vna->channel(c).isCalibrated())
            result << c;
    }
    return result;
}

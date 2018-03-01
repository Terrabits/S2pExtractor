#include "statuswidget.h"
#include "ui_statuswidget.h"

using namespace RsaToolbox;

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    showErrorLabel();
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::showErrorLabel() {
    ui->errorLabel->show();
    ui->progressBar->hide();
}
void StatusWidget::showProgressBar() {
    ui->progressBar->show();
    ui->errorLabel->hide();
    ui->errorLabel->clearMessage();
}

ErrorLabel *StatusWidget::errorLabel() {
    return ui->errorLabel;
}
QProgressBar *StatusWidget::progressBar() {
    return ui->progressBar;
}

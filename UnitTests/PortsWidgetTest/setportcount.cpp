#include "setportcount.h"
#include "ui_setportcount.h"

#include <General.h>
#include <QDebug>

SetPortCount::SetPortCount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetPortCount)
{
    ui->setupUi(this);
}

SetPortCount::~SetPortCount()
{
    delete ui;
}

void SetPortCount::on_spinBox_valueChanged(int arg1)
{
    qDebug() << "setting ports: " << arg1;
    QVector<uint> ports;
    if (arg1 > 0) {
        ports = RsaToolbox::range(uint(1), uint(arg1));
    }
    emit portsChanged(ports);
}

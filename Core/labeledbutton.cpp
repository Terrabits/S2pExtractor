#include "labeledbutton.h"
#include "ui_labeledbutton.h"

LabeledButton::LabeledButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabeledButton)
{
    ui->setupUi(this);
    setFocusProxy(ui->button);
    connect(ui->button, SIGNAL(clicked(bool)),
            this, SIGNAL(buttonClicked()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(textChanged(QString)));
}

LabeledButton::~LabeledButton()
{
    delete ui;
}

void LabeledButton::on_button_clicked()
{

}

void LabeledButton::clearLabel() {
    ui->lineEdit->clear();
}

void LabeledButton::setLabel(const QString &text) {
    ui->lineEdit->setText(text);
}
QString LabeledButton::labelText() const {
    return ui->lineEdit->text();
}

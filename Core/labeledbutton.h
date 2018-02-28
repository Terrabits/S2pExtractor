#ifndef LABELEDBUTTON_H
#define LABELEDBUTTON_H

#include <QString>
#include <QWidget>

namespace Ui {
class LabeledButton;
}

class LabeledButton : public QWidget
{
    Q_OBJECT

public:
    explicit LabeledButton(QWidget *parent = 0);
    ~LabeledButton();

    void clearLabel();
    void setLabel(const QString &text);
    QString labelText() const;

signals:
    void buttonClicked();
    void textChanged(QString text);

private slots:
    void on_button_clicked();

private:
    Ui::LabeledButton *ui;
};

#endif // LABELEDBUTTON_H

#ifndef SETPORTCOUNT_H
#define SETPORTCOUNT_H

#include <QVector>
#include <QWidget>

namespace Ui {
class SetPortCount;
}

class SetPortCount : public QWidget
{
    Q_OBJECT

public:
    explicit SetPortCount(QWidget *parent = 0);
    ~SetPortCount();

signals:
    void portsChanged(QVector<uint> ports);

private slots:
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::SetPortCount *ui;
};

#endif // SETPORTCOUNT_H

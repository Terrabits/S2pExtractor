#ifndef CALWIDGET_H
#define CALWIDGET_H


// Project
#include "CalibrationSource.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QWidget>


namespace Ui {
class CalWidget;
}

class CalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalWidget(QWidget *parent = 0);
    ~CalWidget();

    void setVna(RsaToolbox::Vna *vna);

    CalibrationSource source() const;
    void setSource(CalibrationSource source);

private slots:
    void on_pushButton_clicked();

private:
    Ui::CalWidget *ui;
    void updateDisplay();

    bool isVna() const;
    RsaToolbox::Vna  *_vna;
    QStringList calGroups();
    QVector<uint> channels();

    CalibrationSource _source;
};

#endif // CALWIDGET_H

#ifndef CALWIDGET_H
#define CALWIDGET_H


// Project
#include "CalibrationSource.h"
#include "labeledbutton.h"

// RsaToolbox
#include <Vna.h>

// Qt
#include <QWidget>

class CalWidget : public LabeledButton
{
    Q_OBJECT

public:
    explicit CalWidget(QWidget *parent = 0);
    ~CalWidget();

    void setVna(RsaToolbox::Vna *vna);

    CalibrationSource source() const;
    void setSource(CalibrationSource source);

signals:
    void sourceChanged(CalibrationSource source);

private slots:
    void showDialog();

private:
    void updateLabel();

    bool isVna() const;
    RsaToolbox::Vna  *_vna;
    QStringList calGroups();
    QVector<uint> calibratedChannels();

    CalibrationSource _source;
};

#endif // CALWIDGET_H

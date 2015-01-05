#ifndef GETCALIBRATIONDIALOG_H
#define GETCALIBRATIONDIALOG_H


// Project
#include "Calibration.h"

// RsaToolbox
#include "Vna.h"

// Qt
#include <QVector>
#include <QStringList>
#include <QDialog>


namespace Ui {
class getCalibrationDialog;
}

class getCalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getCalibrationDialog(QWidget *parent = 0);
    ~getCalibrationDialog();

    bool isVna() const;
    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    void clearDefault();
    bool isDefault() const;
    Calibration defaultCalibration() const;
    void setDefault(Calibration cal);

    Calibration calibration() const;

signals:
    void changed(Calibration cal);

public slots:
    virtual int exec();
    virtual void accept();
    virtual void reject();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_channels_itemSelectionChanged();
    void on_calGroups_itemSelectionChanged();

private:
    Ui::getCalibrationDialog *ui;

    RsaToolbox::Vna *_vna;
    void updateUi();

    QVector<uint> _channels;
    QStringList _calGroups;
    Calibration _default;
    Calibration _selection;

    Calibration _calibration;

    void reset();
    void unselectAll();
    void select(const Calibration &cal);

};

#endif // GETCALIBRATIONDIALOG_H

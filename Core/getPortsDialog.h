#ifndef GETPORTSDIALOG_H
#define GETPORTSDIALOG_H


// RsaToolbox
#include "Vna.h"

// Qt
#include <QDialog>


namespace Ui {
class getPortsDialog;
}

class getPortsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getPortsDialog(QWidget *parent = 0);
    ~getPortsDialog();

    bool isVna() const;
    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    bool isDefault() const;
    void clearDefault();
    RsaToolbox::Ports defaultPorts() const;
    void setDefaultPorts(RsaToolbox::Ports ports);

    RsaToolbox::Ports ports() const;

signals:
    void changed(RsaToolbox::Ports ports);

public slots:
    virtual int exec();
    virtual void accept();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void on_selectAll_toggled(bool checked);

private:
    Ui::getPortsDialog *ui;

    RsaToolbox::Vna *_vna;
    void updateUi();

    RsaToolbox::Ports _vnaPorts;
    RsaToolbox::Ports _default;
    RsaToolbox::Ports selection();

    RsaToolbox::Ports _ports;

    void reset();
    void checkAll();
    void uncheckAll();
    void check(const RsaToolbox::Ports &ports);
};

#endif // GETPORTSDIALOG_H

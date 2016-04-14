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
    QVector<uint> defaultPorts() const;
    void setDefaultPorts(QVector<uint> ports);

    QVector<uint> ports() const;

signals:
    void changed(QVector<uint> ports);

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

    QVector<uint> _vnaPorts;
    QVector<uint> _default;
    QVector<uint> selection();

    QVector<uint> _ports;

    void reset();
    void checkAll();
    void uncheckAll();
    void check(const QVector<uint> &ports);
};

#endif // GETPORTSDIALOG_H

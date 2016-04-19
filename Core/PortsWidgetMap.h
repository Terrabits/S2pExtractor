#ifndef PORTSWIDGETMAP_H
#define PORTSWIDGETMAP_H


// Project
#include "getPortsDialog.h"

// RsaToolbox
#include "Vna.h"

// Qt
#include <QObject>
#include <QSharedPointer>
#include <QLineEdit>
#include <QPushButton>


class PortsWidgetMap : public QObject
{
    Q_OBJECT

public:
    explicit PortsWidgetMap(QWidget *parent = 0);
    ~PortsWidgetMap();

    bool isPorts() const;
    QVector<uint> ports() const;
    void setPorts(QVector<uint> ports);

    bool isVna() const;
    RsaToolbox::Vna *vna() const;
    void setVna(RsaToolbox::Vna *vna);

    bool isLineEdit() const;
    QLineEdit *lineEdit() const;
    void setLineEdit(QLineEdit *edit);

    bool isPushButton() const;
    QPushButton *pushButton() const;
    void setPushButton(QPushButton *button);

public slots:
    void getPorts();

private:
    QVector<uint> _ports;
    getPortsDialog _dialog;

    RsaToolbox::Vna *_vna;
    QLineEdit *_lineEdit;
    QPushButton *_pushButton;
    void connectView();
    void disconnectView();
    void updateView();
};

#endif // PORTSWIDGETMAP_H

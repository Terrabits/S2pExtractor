#ifndef PORTSLINEEDIT_H
#define PORTSLINEEDIT_H


// RsaToolbox
#include "General.h"

// Qt
#include <QLineEdit>
#include <QVector>

class PortsLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    PortsLineEdit(QWidget *parent = 0);

    QVector<uint> ports();
    QVector<uint> sortedPorts();

private:
    void setValidator(const QValidator *validator);

    QVector<uint> _ports;
    void parsePortEntry(QString word);
    void addPort(uint addPort);
    void addPorts(uint start, uint stop);
};

#endif

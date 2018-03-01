#ifndef SAVEDIRECTORY_H
#define SAVEDIRECTORY_H

#include "labeledbutton.h"

#include <LastPath.h>

#include <QString>
#include <QWidget>

class SaveDirectory : public LabeledButton
{
    Q_OBJECT
public:
    explicit SaveDirectory(QWidget *parent = 0);

    QString path() const;

public slots:
    void setPath(QString path);

signals:
    void directoryChanged(QString newDirectory);

private slots:
    void showDialog();

private:
    RsaToolbox::LastPath _lastPath;
    QString              _path;

    void updateDisplay();
};

#endif // SAVEDIRECTORY_H

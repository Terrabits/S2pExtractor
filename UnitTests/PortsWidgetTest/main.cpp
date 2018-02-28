#include "portswidget.h"
#include "setportcount.h"
#include <QApplication>
#include <QDebug>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVector<uint> vector;
    // does this cause problems?
    foreach (const uint _uint, vector) {
        qDebug() << "nope! " << _uint;
    }

    PortsWidget w;
    w.show();

    SetPortCount portCount;
    portCount.show();

    QObject::connect(&portCount, SIGNAL(portsChanged(QVector<uint>)),
            &w, SLOT(setPorts(QVector<uint>)));

    return a.exec();
}

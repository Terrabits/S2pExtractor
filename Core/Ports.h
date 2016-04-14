#ifndef PORTS_H
#define PORTS_H


// Qt
#include <QVector>
#include <QSharedPointer>


typedef QSharedPointer<QVector<uint>> SharedPorts;
SharedPorts newPorts();


#endif // PORTS_H

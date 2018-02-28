#ifndef COMMONPORTS_H
#define COMMONPORTS_H

#include "Channel.h"
#include <QVector>

QVector<uint> commonPorts(Channel *outerChannel, Channel *innerChannel);

#endif // COMMONPORTS_H

#include "saveresults.h"

// RsaToolbox
#include <Touchstone.h>
using namespace RsaToolbox;

// stdlib
#include <algorithm>

SaveResults::SaveResults(Calculate *calculate, const QMap<uint, QString> &filenames, QObject *parent) :
    QObject(parent),
    _calculate(calculate),
    _filenames(filenames)
{

}

void SaveResults::run() {
    emit starting();
    // Process ports, in order
    QList<uint> ports = _filenames.keys();
    std::sort(ports.begin(), ports.end());
    foreach (const uint port, ports) {
        const QString filename = _filenames[port];
        emit processingFile(port, filename);
        if (!Touchstone::write(_calculate->result(port), filename)) {
            emit error(port, filename);
            break;
        }
    }
    emit finished();
}

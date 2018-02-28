#include "fixfilenameextensions.h"

void fixFilenameExtension(QString &filename) {
    if (!filename.endsWith(".s2p", Qt::CaseInsensitive)) {
        filename += ".s2p";
    }
}

void fixFilenameExtensions(QStringList &filenames) {
    for (int i = 0; i < filenames.size(); i++) {
        fixFilenameExtension(filenames[i]);
    }
}
void fixFilenameExtensions(QMap<uint, QString> &filenames) {
    const QList<uint> ports = filenames.keys();
    foreach (const uint port, ports) {
        fixFilenameExtension(filenames[port]);
    }
}

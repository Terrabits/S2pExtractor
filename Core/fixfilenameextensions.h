#ifndef FIXFILENAMEEXTENSIONS_H
#define FIXFILENAMEEXTENSIONS_H

#include <QMap>
#include <QString>
#include <QStringList>

void fixFilenameExtension(QString &filename);
void fixFilenameExtensions(QStringList &filenames);
void fixFilenameExtensions(QMap<uint, QString> &filenames);

#endif // FIXFILENAMEEXTENSIONS_H

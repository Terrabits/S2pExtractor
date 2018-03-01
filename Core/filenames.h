#ifndef FILENAMES_H
#define FILENAMES_H

#include <QMap>
#include <QRegExp>
#include <QString>

QRegExp filenameRegex();
bool isValidFilename(const QString &filename);
QString addPath(const QString &filename, const QString &path);
QMap<uint, QString> addPaths(const QMap<uint, QString> &filenames, const QString &path);

QString convertToWindows(const QString &path);

#endif // FILENAMES_H

#include "filenames.h"

#include <QDir>
#include <QStringBuilder>

QRegExp filenameRegex() {
    const QString onlyChar   = "[\\w-\\\\(\\)\\[\\]&%#@! ]";
    const QString endChar    = "[\\w-\\\\(\\)\\[\\]&%#@! ]";
    const QString middleChars = "[\\w-\\.\\(\\)\\[\\]&%#@! ]*";
    return QRegExp(onlyChar % "|(" % endChar % middleChars % endChar % ")");
}
bool isValidFilename(const QString &filename) {
    return filenameRegex().exactMatch(filename);
}

QString addPath(const QString &filename, const QString &path) {
    return convertToWindows(QDir(path).filePath(filename));
}
QMap<uint, QString> addPaths(const QMap<uint, QString> &filenames, const QString &path) {
    QMap<uint, QString> result;
    foreach (const uint port, filenames.keys()) {
        result[port] = addPath(filenames[port], path);
    }
    return result;
}
QString convertToWindows(const QString &path) {
    QString _path = path;
    return _path.replace("/", "\\");
}

#ifndef SAVERESULTS_H
#define SAVERESULTS_H

#include "Calculate.h"
#include <QMap>
#include <QObject>
#include <QString>

class SaveResults : public QObject
{
    Q_OBJECT
public:
    explicit SaveResults(Calculate *calculate, const QMap<uint, QString> &filenames, QObject *parent = 0);

signals:
    void starting();
    void processingFile(uint port, QString filename);
    void error(uint port, QString filename);
    void finished();

public slots:
    void run();

private:
    Calculate *_calculate;
    QMap<uint, QString> _filenames;
};

#endif // SAVERESULTS_H

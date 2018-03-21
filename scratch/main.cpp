
#include "CalibrationSource.h"
#include "Corrections.h"
#include "Settings.h"

#include <Definitions.h>
#include <Vna.h>
using namespace RsaToolbox;

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <cassert>

QString sign(double value) {
    if (value < 0) {
        return "";
    }
    return "+";
}

void write(const ComplexRowVector &data, const QString &filename) {
    QFile file(filename);
    assert(file.open(QFile::WriteOnly));
    QTextStream stream(&file);
    for (uint i = 0; i < data.size(); i++) {
        QString line = "(%1%2%3j)\n";
        line = line.arg(data[i].real());
        line = line.arg(sign(data[i].imag()));
        line = line.arg(data[i].imag());
        stream << line;
    }
    stream.flush();
    file.flush();
    file.close();
}
void write(const QRowVector &data, const QString filename) {
    QFile file(filename);
    assert(file.open(QFile::WriteOnly));
    QTextStream stream(&file);
    for (int i = 0; i < data.size(); i++) {
        stream << data[i] << "\n";
    }
    stream.flush();
    file.flush();
    file.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir root(SOURCE_DIR);
    root.cd("Fred 3");
    root.cd("port1");
    foreach(const QString filename, root.entryList(QDir::Files)) {
        root.remove(filename);
    }

    Vna vna(CONNECTION_TYPE, INSTRUMENT_ADDRESS);
    vna.preset();
    vna.clearStatus();
    vna.pause();

    CalibrationSource outerSource("uosm");
    Corrections outer(2, 1, outerSource, &vna);
    assert(outer.isPort1Corrections());
    write(outer.frequencies_Hz(), root.filePath("outer_frequency.csv"));
    assert(!vna.isError());
    write(outer.directivity1(), root.filePath("outer_directivity.csv"));
    assert(!vna.isError());
    write(outer.sourceMatch1(), root.filePath("outer_sourcematch.csv"));
    assert(!vna.isError());
    write(outer.reflectionTracking1(), root.filePath("outer_refltrack.csv"));
    assert(!vna.isError());

    CalibrationSource innerSource("guidUOSMOS12");
    Corrections inner(2, 1, innerSource, &vna);
    assert(inner.isPort1Corrections());
    write(inner.frequencies_Hz(), root.filePath("inner_frequency.csv"));
    assert(!vna.isError());
    write(inner.directivity1(), root.filePath("inner_directivity.csv"));
    assert(!vna.isError());
    write(inner.sourceMatch1(), root.filePath("inner_sourcematch.csv"));
    assert(!vna.isError());
    write(inner.reflectionTracking1(), root.filePath("inner_refltrack.csv"));
    assert(!vna.isError());

    return 0;
}

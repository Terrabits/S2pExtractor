#include "CalculateTest.h"


// Project
#include "Calculate.h"

// RsaToolbox
#include <Vna.h>
#include <Log.h>
#include <Test.h>
#include <Touchstone.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>
#include <QTest>
#include <QDebug>


CalculateTest::CalculateTest(QObject *parent) :
    VnaTestClass(parent)
{
    _logDir = QDir(SOURCE_DIR);
    _logDir.mkpath("CalculateTest");
    _logDir.cd("CalculateTest");
    _logDir.mkpath("Znb");
    _logDir.cd("Znb");
    _logDir.mkpath("Logs");
    _logDir.cd("Logs");
    _logFilenames << "1 - UOSM Calculations.txt"
                  << "2 - OSM Calculations.txt";
}
CalculateTest::~CalculateTest() {
}

void CalculateTest::uosmCalculations() {
    QVERIFY(_vna->properties().isZnbFamily());

    QDir calGroupDir = _logDir;
    QVERIFY(calGroupDir.cdUp());
    QVERIFY(calGroupDir.cd("UOSM CalGroup"));
    QVERIFY(calGroupDir.exists());
    QVERIFY(calGroupDir.exists("OuterCal.cal"));
    QVERIFY(calGroupDir.exists("InnerCal.cal"));

    QString outerCal = "OuterCal.cal";
    QString innerCal = "InnerCal.cal";
    _vna->fileSystem().uploadFile(calGroupDir.filePath(outerCal), outerCal, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->fileSystem().uploadFile(calGroupDir.filePath(innerCal), innerCal, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);

    QVector<uint> ports;
    ports << 1 << 2;
    CalibrationSource outerSource(outerCal);
    CalibrationSource innerSource(innerCal);

    Calculate calculate(outerSource, innerSource, ports, _vna.data());
    QString msg;
    calculate.isError(msg);
    QCOMPARE(QString(), msg);
    QVERIFY(!calculate.isError());
    QCOMPARE(calculate.result(1).points(), uint(911));
    QCOMPARE(calculate.result(2).points(), uint(911));
    QVERIFY(!_vna->isError());

    Touchstone::write(calculate.result(1), calGroupDir.filePath("Port 1"));
    Touchstone::write(calculate.result(2), calGroupDir.filePath("Port 2"));
}
void CalculateTest::osmCalculations() {
    qDebug() << "Not implemented";
}


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

    _calGroupDir = _logDir;
    _calGroupDir.cdUp();
    _calGroupDir.cd("CalGroups");

    _resultsDir = _logDir;
    _resultsDir.cdUp();
    _resultsDir.cd("Results");
}
CalculateTest::~CalculateTest() {
}

void CalculateTest::initTestCase() {
    // Prepare results folder
    if (!_resultsDir.exists()) {
        QDir dir(_resultsDir);
        dir.cdUp();
        dir.mkpath(_resultsDir.dirName());
    }
    else {
        foreach (QString file, _resultsDir.entryList(QDir::Files)) {
            _logDir.remove(file);
        }
    }

    _initTestCase();

    _vna.reset(new Vna(_connectionType, _address));

    // Get all test cal groups
    QDir::Filters filters = QDir::Files;
    QDir::SortFlags sort = QDir::Name | QDir::IgnoreCase;
    QStringList nameFilters;
    nameFilters << "*.cal";
    _calGroups = _calGroupDir.entryList(nameFilters, filters, sort);

    // Upload Cal groups
    foreach (const QString &file, _calGroups) {
        const QString filePath = _calGroupDir.filePath(file);
        _vna->fileSystem().uploadFile(filePath, file, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    }
    _vna->pause(10000);

    _vna.reset();
}
void CalculateTest::cleanupTestCase() {
    _vna.reset(new Vna(_connectionType, _address));
    if (_vna->isConnected()) {
        // Delete cal groups
        QString dir = _vna->fileSystem().directory();
        _vna->fileSystem().changeDirectory(VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
        _vna->fileSystem().deleteFiles(_calGroups);
        _vna->pause();
        _vna->fileSystem().changeDirectory(dir);
    }

    _cleanupTestCase();
}

void CalculateTest::uosmCalculations() {
    QVERIFY(_vna->properties().isZnbFamily());

    QString outerCal = "UOSM Outer.cal";
    QString innerCal = "UOSM Inner.cal";

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

    Touchstone::write(calculate.result(1), _resultsDir.filePath("UOSM Port 1"));
    Touchstone::write(calculate.result(2), _resultsDir.filePath("UOSM Port 2"));
}
void CalculateTest::osmCalculations() {
    QVERIFY(_vna->properties().isZnbFamily());

    QString outerCal = "OSM Outer.cal";
    QString innerCal = "OSM Inner.cal";

    QVector<uint> ports;
    ports << 1 << 2;
    CalibrationSource outerSource(outerCal);
    CalibrationSource innerSource(innerCal);

    Calculate calculate(outerSource, innerSource, ports, _vna.data());
    QString msg;
    calculate.isError(msg);
    QCOMPARE(QString(), msg);
    QVERIFY(!calculate.isError());
    QCOMPARE(calculate.result(1).points(), uint(91));
    QCOMPARE(calculate.result(2).points(), uint(91));
    QVERIFY(!_vna->isError());

    Touchstone::write(calculate.result(1), _resultsDir.filePath("OSM Port 1"));
    Touchstone::write(calculate.result(2), _resultsDir.filePath("OSM Port 2"));
}


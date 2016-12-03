#include "CalculateTest.h"


// Project
#include "Calculate.h"
#include "CalibrationSource.h"
#include "Error.h"

// RsaToolbox
#include <Vna.h>
#include <Log.h>
#include <Test.h>
#include <Touchstone.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>
#include <QString>
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
    _logFilenames << "1  - isReady No error.txt"
                  << "2  - isReady No ports.txt"
                  << "3  - isReady Outer ports.txt"
                  << "4  - isReady Inner ports.txt"
                  << "5  - isReady Extra port.txt"
                  << "6  - isReady No common ports.txt"
                  << "7  - isReady empty outer.txt"
                  << "8  - isReady empty inner.txt"
                  << "9  - isReady Missing outer cal.txt"
                  << "10 - isReady Missing inner cal.txt"
                  << "11 - isReady Missing outer channel.txt"
                  << "12 - isReady Missing inner channel.txt"
                  << "13 - isReady Missing outer channel cal.txt"
                  << "14 - isReady Missing inner channel cal.txt"
                  << "15 - isReady frequency.txt"
                  << "16 - UOSM calculations.txt"
                  << "17 - OSM calculations.txt";

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

void CalculateTest::isReady_data() {
    QTest::addColumn<CalibrationSource>("outer");
    QTest::addColumn<CalibrationSource>("inner");
    QTest::addColumn<QVector<uint>>("ports");
    QTest::addColumn<Error::Code>("errorCode");

    CalibrationSource outer("outer_ready");
    CalibrationSource inner("inner_ready");

    QVector<uint> ports;
    ports << 1 << 2;

    // To generate errors
    QVector<uint> noPorts;
    QVector<uint> threePorts;
    threePorts << 1 << 2 << 3;

    CalibrationSource outerMissingPort("outer_ready_missing_port");
    CalibrationSource innerMissingPort("inner_ready_missing_port");
    CalibrationSource innerWrongPorts("inner_ready_ports3-4");
    CalibrationSource emptyCal;
    CalibrationSource missingCal("missing_cal");
    CalibrationSource missingChannelCal(1);
    CalibrationSource missingChannel(10);
    CalibrationSource innerWrongFreq("inner_ready_freq");

    // Tests
    QTest::newRow("No error")    << outer            << inner            << ports << Error::Code::None;

    // Ports
    ports.clear();
    QTest::newRow("No ports")    << outer            << inner            << ports << Error::Code::Ports;
    ports << 1 << 2;
    QTest::newRow("Outer ports") << outerMissingPort << inner            << ports << Error::Code::Ports;
    QTest::newRow("Inner ports") << outer            << innerMissingPort << ports << Error::Code::Ports;
    ports << 3;
    QTest::newRow("Extra port")  << outer            << inner            << ports << Error::Code::Ports;
    ports.clear();
    ports << 1 << 2;
    QTest::newRow("No common ports") << outer        << innerWrongPorts  << ports << Error::Code::Other;

    // Calibration
    QTest::newRow("Empty outer")               << emptyCal          << inner             << ports << Error::Code::OuterCalibration;
    QTest::newRow("Empty inner")               << outer             << emptyCal          << ports << Error::Code::InnerCalibration;
    QTest::newRow("Missing outer cal")         << missingCal        << inner             << ports << Error::Code::OuterCalibration;
    QTest::newRow("Missing inner cal")         << outer             << missingCal        << ports << Error::Code::InnerCalibration;
    QTest::newRow("Missing outer channel")     << missingChannel    << inner             << ports << Error::Code::OuterCalibration;
    QTest::newRow("Missing inner channel")     << outer             << missingChannel    << ports << Error::Code::InnerCalibration;
    QTest::newRow("Missing outer channel cal") << missingChannelCal << inner             << ports << Error::Code::OuterCalibration;
    QTest::newRow("Missing inner channel cal") << outer             << missingChannelCal << ports << Error::Code::InnerCalibration;

    QTest::newRow("freq")                      << outer             << innerWrongFreq    << ports << Error::Code::Other;
}
void CalculateTest::isReady() {
    QFETCH(CalibrationSource, outer);
    QFETCH(CalibrationSource, inner);
    QFETCH(QVector<uint>, ports);
    QFETCH(Error::Code, errorCode);

    Calculate calculate(outer, inner, ports, _vna.data());

    Error error;
    QCOMPARE(calculate.isReady(error), errorCode == Error::Code::None);
    QCOMPARE(error.code,               errorCode);
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
    calculate.run();
    QCOMPARE(QString(), calculate.error().message);
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
    calculate.run();
    QCOMPARE(QString(), calculate.error().message);
    QVERIFY(!calculate.isError());
    QCOMPARE(calculate.result(1).points(), uint(91));
    QCOMPARE(calculate.result(2).points(), uint(91));
    QVERIFY(!_vna->isError());

    Touchstone::write(calculate.result(1), _resultsDir.filePath("OSM Port 1"));
    Touchstone::write(calculate.result(2), _resultsDir.filePath("OSM Port 2"));
}


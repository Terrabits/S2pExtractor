#include "ChannelTest.h"


// Project
#include "CalibrationSource.h"
#include "Channel.h"

// RsaToolbox
using namespace RsaToolbox;


ChannelTest::ChannelTest(QObject *parent) :
    VnaTestClass(parent)
{
    _logDir = QDir(SOURCE_DIR);
    _logDir.mkpath("ChannelTest");
    _logDir.cd("ChannelTest");
    _logDir.mkpath("Znb");
    _logDir.cd("Znb");
    _logDir.mkpath("Logs");
    _logDir.cd("Logs");
    _logFilenames << "1 - Empty.txt"
                  << "2 - Valid cal.txt"
                  << "3 - Valid channel.txt"
                  << "4 - No cal.txt"
                  << "5 - No channel cal.txt"
                  << "6 - No channel.txt";

    _calGroupDir = _logDir;
    _calGroupDir.cdUp();
    _calGroupDir.cd("CalGroups");
}

ChannelTest::~ChannelTest()
{

}

void ChannelTest::initTestCase() {
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
void ChannelTest::cleanupTestCase() {
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

void ChannelTest::init() {
    VnaTestClass::_init();
    _vna->createChannel(2);
    _vna->channel(2).setCalGroup("calibration.cal");
    _vna->channel(2).dissolveCalGroupLink();
}

void ChannelTest::isReady_data() {
    QTest::addColumn<CalibrationSource>("source");
    QTest::addColumn<bool>("isReady");

    CalibrationSource empty;
    CalibrationSource validCal("calibration.cal");
    CalibrationSource validChannel(2);
    CalibrationSource noChannelCal(1);
    CalibrationSource noChannel(10);
    CalibrationSource noCal("missing_calibration.cal");

    QTest::newRow("Empty") << empty << false;
    QTest::newRow("Valid cal") << validCal << true;
    QTest::newRow("Valid channel") << validChannel << true;
    QTest::newRow("No cal") << noCal << false;
    QTest::newRow("No channel cal") << noChannelCal << false;
    QTest::newRow("No channel") << noChannel << false;
}
void ChannelTest::isReady() {
    QFETCH(CalibrationSource, source);
    QFETCH(bool, isReady);

    Channel channel(source, _vna.data());
    QCOMPARE(channel.isReady(), isReady);
}

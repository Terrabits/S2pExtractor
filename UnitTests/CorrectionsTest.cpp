#include "CorrectionsTest.h"


// Project
#include "Corrections.h"

// RsaToolbox
#include <General.h>
using namespace RsaToolbox;

// Qt
#include <QTest>


CorrectionsTest::CorrectionsTest(QObject *parent) :
    VnaTestClass(parent)
{
    _logDir.setPath(SOURCE_DIR);
    _logDir.mkpath("CorrectionsTest");
    _logDir.cd("CorrectionsTest");
    Vna vna(ConnectionType::VisaTcpSocketConnection, "127.0.0.1::5025");
    if (vna.properties().isZvaFamily()) {
        _logDir.mkpath("Zva");
        _logDir.cd("Zva");
    }
    else {
        _logDir.mkpath("Znb");
        _logDir.cd("Znb");
    }
    _calGroupDir.setPath(_logDir.filePath("CalGroups"));
    _logDir.mkpath("Logs");
    _logDir.cd("Logs");

    _logFilenames << "1 - Ports 1,2.txt"
                  << "2 - Ports 1-4.txt"
                  << "3 - Ports 1-4 OSM.txt"
                  << "4 - Port  1   OSM.txt"
                  << "5 - Ports 1,2 SwMat.txt"
                  << "6 - Ports 1-4 SwMat.txt"
                  << "7 - Ports 1-4 SwMat OSM.txt"
                  << "8 - Port  1   SwMat OSM.txt";
}
CorrectionsTest::~CorrectionsTest()
{
    //
}

void CorrectionsTest::ports12() {
    removeSwitchMatrices();

    QString calGroup = "Ports 1,2.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));

    Ports testPorts = range(uint(1), _vna->testPorts());
    corrections = Corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
}
void CorrectionsTest::ports1to4() {
    removeSwitchMatrices();

    QString calGroup = "Ports 1-4.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity2: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    corrections = Corrections(3, 4, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity3: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity3: ");
    _log->printLine(toString(corrections.directivity2(), ", "));
}
void CorrectionsTest::ports1to4Osm() {
    removeSwitchMatrices();

    QString calGroup = "Ports 1-4 OSM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity2: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    corrections = Corrections(3, 4, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity3: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity4: ");
    _log->printLine(toString(corrections.directivity2(), ", "));
}
void CorrectionsTest::port1Osm() {
    removeSwitchMatrices();

    QString calGroup = "Port 1 OSM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Ports testPorts = range(uint(1), _vna->testPorts());
    Corrections corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
}

void CorrectionsTest::ports12SwMat() {
    if (_vna->properties().isZvaFamily())
        return;

    addSwitchMatrix();

    QString calGroup = "Ports 1,2 SwM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity2: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    Ports testPorts = range(uint(1), _vna->testPorts());
    corrections = Corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));

    _vna->isError();
    _vna->clearStatus();

    removeSwitchMatrices();
}
void CorrectionsTest::ports1to4SwMat() {
    if (_vna->properties().isZvaFamily())
        return;

    addSwitchMatrix();

    QString calGroup = "Ports 1-4 SwM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity2: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    corrections = Corrections(3, 4, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity3: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity4: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    removeSwitchMatrices();
}
void CorrectionsTest::ports1to4SwMatOsm() {
    if (_vna->properties().isZvaFamily())
        return;

    addSwitchMatrix();

    QString calGroup = "Ports 1-4 SwM OSM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Corrections corrections(1, 2, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity2: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    corrections = Corrections(3, 4, _vna->channel().corrections());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(corrections.isPort2Corrections());
    QCOMPARE(corrections.directivity2().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity3: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
    _log->printLine("directivity4: ");
    _log->printLine(toString(corrections.directivity2(), ", "));

    removeSwitchMatrices();
}
void CorrectionsTest::port1SwMatOsm() {
    if (_vna->properties().isZvaFamily())
        return;

    addSwitchMatrix();

    QString calGroup = "Port 1 SwM OSM.cal";
    _vna->fileSystem().uploadFile(_calGroupDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    Ports testPorts = range(uint(1), _vna->testPorts());
    Corrections corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));

    removeSwitchMatrices();
}

void CorrectionsTest::addSwitchMatrix() {
    if (_vna->properties().isZvaFamily())
        return;

     removeSwitchMatrices();

    PortMap vnaToTestPortMap;
    vnaToTestPortMap[1] = 1;
    vnaToTestPortMap[2] = 2;

    PortMap matrixToVnaPortMap;
    matrixToVnaPortMap[1] = 3;
    matrixToVnaPortMap[2] = 4;

    PortMap matrixToTestPortMap;
    matrixToTestPortMap[1]  = 3;
    matrixToTestPortMap[2]  = 4;
    matrixToTestPortMap[3]  = 5;
    matrixToTestPortMap[4]  = 6;
    matrixToTestPortMap[5]  = 7;
    matrixToTestPortMap[6]  = 8;
    matrixToTestPortMap[7]  = 9;
    matrixToTestPortMap[8]  = 10;
    matrixToTestPortMap[9]  = 11;
    matrixToTestPortMap[10] = 12;
    matrixToTestPortMap[11] = 13;
    matrixToTestPortMap[12] = 14;

    _vna->registerSwitchMatrix(VnaSwitchMatrix::Driver::ZN_Z84_22,
                               VnaSwitchMatrix::ConnectionType::Lan,
                               "127.0.0.1");
    _vna->includeAllSwitchMatricesInSetup();
    _vna->beginSwitchMatrixSetup();
    _vna->setVnaTestPorts(vnaToTestPortMap);
    _vna->switchMatrix(1).setConnectionsToVna(matrixToVnaPortMap);
    _vna->switchMatrix(1).setTestPorts(matrixToTestPortMap);
    _vna->endSwitchMatrixSetup();
}
void CorrectionsTest::removeSwitchMatrices() {
    if (_vna->properties().isZvaFamily())
        return;

    _vna->excludeAllSwitchMatricesFromSetup();
    _vna->unregisterAllSwitchMatrices();
}

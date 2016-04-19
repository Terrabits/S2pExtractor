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
    _logDir.cd("CorrectionsTest");
    Vna vna(ConnectionType::VisaTcpSocketConnection, "127.0.0.1::5025");
    if (vna.properties().isZvaFamily())
        _logDir.cd("Zva");
    else
        _logDir.cd("Znb");

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
    if (_vna->switchMatrices()) {
        qDebug() << "Can\'t test with switch matrix";
        return;
    }

    QString calGroup = "Ports 1,2.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    corrections = Corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));

    _vna->isError();
    _vna->clearStatus();
}
void CorrectionsTest::ports1to4() {
    if (_vna->switchMatrices()) {
        qDebug() << "Can\'t test with switch matrix";
        return;
    }

    QString calGroup = "Ports 1-4.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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
    if (_vna->switchMatrices()) {
        qDebug() << "Can\'t test with switch matrix";
        return;
    }

    QString calGroup = "Ports 1-4 OSM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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
    if (_vna->switchMatrices()) {
        qDebug() << "Can\'t test with switch matrix";
        return;
    }

    QString calGroup = "Port 1 OSM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    Corrections corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
}

void CorrectionsTest::ports12SwMat() {
    if (!_vna->switchMatrices()) {
        qDebug() << "Can\'t test without switch matrix";
        return;
    }

    QString calGroup = "Ports 1,2 SwM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    corrections = Corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));

    _vna->isError();
    _vna->clearStatus();
}
void CorrectionsTest::ports1to4SwMat() {
    if (!_vna->switchMatrices()) {
        qDebug() << "Can\'t test without switch matrix";
        return;
    }

    QString calGroup = "Ports 1-4 SwM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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
void CorrectionsTest::ports1to4SwMatOsm() {
    if (!_vna->switchMatrices()) {
        qDebug() << "Can\'t test without switch matrix";
        return;
    }

    QString calGroup = "Ports 1-4 SwM OSM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
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
void CorrectionsTest::port1SwMatOsm() {
    if (!_vna->switchMatrices()) {
        qDebug() << "Can\'t test without switch matrix";
        return;
    }

    QString calGroup = "Port 1 SwM OSM.cal";
    _vna->fileSystem().uploadFile(_logDir.filePath(calGroup), calGroup, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(calGroup);
    QVERIFY(!_vna->channel().calGroup().isEmpty());
    QVERIFY(_vna->channel().isCalibrated());

    QVector<uint> testPorts = range(uint(1), _vna->testPorts());
    Corrections corrections(1, testPorts, _vna->channel().corrections(), _vna.data());
    QVERIFY(corrections.isPort1Corrections());
    QCOMPARE(corrections.directivity1().size(), uint(201));
    QVERIFY(!_vna->isError());

    _log->printLine("directivity1: ");
    _log->printLine(toString(corrections.directivity1(), ", "));
}

#include "CalculateTest.h"


// RsaToolbox
#include <Vna.h>
#include <Log.h>
#include <Test.h>
using namespace RsaToolbox;

// Qt
#include <QFileInfo>
#include <QTest>
#include <QDebug>


CalculateTest::CalculateTest(QObject *parent) :
    VnaTestClass(parent)
{
    _logDir.setPath(QDir(SOURCE_DIR).filePath("CalculateTest"));
    _logFilenames << "1 - MyTest.txt";
}
CalculateTest::~CalculateTest() {
}


void CalculateTest::myTest() {
    QVERIFY(_vna->isConnected());

    QString filename = "Cal Group 1.cal";
    QString calGroupFile = _logDir.filePath(filename);
    _vna->fileSystem().uploadFile(calGroupFile, filename, VnaFileSystem::Directory::CAL_GROUP_DIRECTORY);
    _vna->channel().setCalGroup(filename);
    QVERIFY(!_vna->isError());
    QCOMPARE(QFileInfo(_vna->channel().calGroupFile()).fileName(), filename);
}

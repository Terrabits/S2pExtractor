#include "CalibrationSourceTest.h"


// Project
#include "CalibrationSource.h"

// Qt
#include <QTest>


CalibrationSourceTest::CalibrationSourceTest(QObject *parent) :
    TestClass(parent)
{

}

CalibrationSourceTest::~CalibrationSourceTest()
{

}

void CalibrationSourceTest::noSource() {
    CalibrationSource source;
    QVERIFY(source.isEmpty());
    QVERIFY(!source.isChannel());
    QVERIFY(!source.isCalGroup());
    QVERIFY(source.displayText().isEmpty());
}
void CalibrationSourceTest::channel() {
    CalibrationSource source(1);
    QVERIFY(!source.isEmpty());
    QVERIFY(source.isChannel());
    QCOMPARE(uint(1), source.channel());
    QVERIFY(!source.isCalGroup());
    QCOMPARE(QString("Channel 1"), source.displayText());

    source.clear();
    QVERIFY(source.isEmpty());

    source.setChannel(2);
    QVERIFY(!source.isEmpty());
    QVERIFY(source.isChannel());
    QCOMPARE(uint(2), source.channel());
    QVERIFY(!source.isCalGroup());
    QCOMPARE(QString("Channel 2"), source.displayText());
}
void CalibrationSourceTest::calGroup() {
    QString calGroup = "My Cal Group";
    CalibrationSource source("My Cal Group");
    QVERIFY(!source.isEmpty());
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(calGroup, source.calGroup());
    QCOMPARE(calGroup, source.displayText());

    source.clear();
    QVERIFY(source.isEmpty());

    source.setCalGroup(calGroup);
    QVERIFY(!source.isEmpty());
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(calGroup, source.calGroup());
    QCOMPARE(calGroup, source.displayText());
}
void CalibrationSourceTest::switchSource() {
    QString calGroup = "My Cal Group";
    CalibrationSource source("My Cal Group");
    QVERIFY(!source.isEmpty());
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(calGroup, source.calGroup());
    QCOMPARE(calGroup, source.displayText());

    source.clear();
    QVERIFY(source.isEmpty());

    source.setChannel(2);
    QVERIFY(!source.isEmpty());
    QVERIFY(source.isChannel());
    QCOMPARE(uint(2), source.channel());
    QVERIFY(!source.isCalGroup());
    QCOMPARE(QString("Channel 2"), source.displayText());

    source.clear();
    QVERIFY(source.isEmpty());

    source.setCalGroup(calGroup);
    QVERIFY(!source.isEmpty());
    QVERIFY(!source.isChannel());
    QVERIFY(source.isCalGroup());
    QCOMPARE(calGroup, source.calGroup());
    QCOMPARE(calGroup, source.displayText());
}

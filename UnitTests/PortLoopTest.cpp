#include "PortLoopTest.h"


// Project
#include "PortLoop.h"

// Qt
#include <QTest>
#include <QVector>


PortLoopTest::PortLoopTest(QObject *parent) :
    TestClass(parent)
{
    //
}

PortLoopTest::~PortLoopTest()
{
    //
}

void PortLoopTest::noPorts() {
    QVector<uint> ports;
    PortLoop loop1(ports, ports);
    QVERIFY(!loop1.isUnprocessedPorts());

    QVector<uint> allPorts;
    allPorts << 1 << 2;
    PortLoop loop2(QVector<uint>(), allPorts);
    QVERIFY(!loop2.isUnprocessedPorts());
}
void PortLoopTest::oneOfTwoPortsProcessed() {
    QVector<uint> ports;
    ports << 1;

    QVector<uint> allPorts;
    allPorts << 1 << 2;

    PortLoop loop(ports, allPorts);
    QVERIFY(loop.isUnprocessedPorts());
    loop.begin();
    QCOMPARE(uint(1), loop.port1());
    QCOMPARE(uint(2), loop.port2());
    QVERIFY(!loop.isPort2Unprocessed());
    loop.usePorts();
    loop.end();
    QVERIFY(!loop.isUnprocessedPorts());
}
void PortLoopTest::twoPortsProcessed() {
    QVector<uint> ports;
    ports << 1 << 2;

    PortLoop loop(ports, ports);
    QVERIFY(loop.isUnprocessedPorts());
    loop.begin();
    QCOMPARE(uint(1), loop.port1());
    QVERIFY(loop.isPort2Unprocessed());
    QCOMPARE(uint(2), loop.port2());
    loop.usePorts();
    loop.end();
    QVERIFY(!loop.isUnprocessedPorts());
}
void PortLoopTest::manyPortsProcessed() {
    QVector<uint> ports;
    ports << 2 << 3 << 4;

    QVector<uint> allPorts;
    allPorts << 1 << 2 << 3 << 4 << 5 << 6;

    PortLoop loop(ports, allPorts);
    QVERIFY(loop.isUnprocessedPorts());
    loop.begin();

    // Ports: 2, 3
    QCOMPARE(uint(2), loop.port1());
    QCOMPARE(uint(3), loop.port2());
    QVERIFY(loop.isPort2Unprocessed());

    // Ports: 2, 4
    QVERIFY(loop.next());
    QCOMPARE(uint(2), loop.port1());
    QCOMPARE(uint(4), loop.port2());
    QVERIFY(loop.isPort2Unprocessed());

    // Ports: 2, 1
    QVERIFY(loop.next());
    QCOMPARE(uint(2), loop.port1());
    QCOMPARE(uint(1), loop.port2());
    QVERIFY(!loop.isPort2Unprocessed());
    loop.usePorts();
    loop.end();

    // Ports: 3, 4
    QVERIFY(loop.isUnprocessedPorts());
    loop.begin();
    QCOMPARE(uint(3), loop.port1());
    QCOMPARE(uint(4), loop.port2());
    QVERIFY(loop.isPort2Unprocessed());
    loop.usePorts();
    loop.end();

    // No ports left
    QVERIFY(!loop.isUnprocessedPorts());
}
void PortLoopTest::cannotProcessPorts() {
    QVector<uint> ports;
    ports << 2 << 3 << 4;

    QVector<uint> allPorts;
    allPorts << 1 << 2 << 3 << 4 << 5 << 6;

    PortLoop loop(ports, allPorts);
    QVERIFY(loop.isUnprocessedPorts());
    loop.begin();
    QCOMPARE(uint(2), loop.port1());
    QCOMPARE(uint(3), loop.port2());

    // Port2: 4
    QVERIFY(loop.next());

    // Port2: 1
    QVERIFY(loop.next());

    // Port2: 5
    QVERIFY(loop.next());

    // Port2: 6
    QVERIFY(loop.next());

    // No ports left
    QVERIFY(!loop.next());
}


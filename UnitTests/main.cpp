

// Tests
#include "CorrectionsTest.h"
#include "CalculateTest.h"
#include "CalibrationSourceTest.h"
#include "ChannelTest.h"
#include "PortLoopTest.h"

// RsaToolbox
#include <General.h>
#include <TestRunner.h>
#include <TestClass.h>
#include <VnaTestClass.h>
using namespace RsaToolbox;

// Qt
#include <QtTest>
#include <QDebug>


int main() {
    TestRunner testRunner;
    testRunner.addTest(new PortLoopTest);
    testRunner.addTest(new CalibrationSourceTest);
    testRunner.addTest(new ChannelTest);
    testRunner.addTest(new CorrectionsTest);
    testRunner.addTest(new CalculateTest);

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}

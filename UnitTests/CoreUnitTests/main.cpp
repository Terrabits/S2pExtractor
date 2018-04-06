

// Tests
#include "CorrectionsTest.h"
#include "CalculateTest.h"
#include "CalibrationSourceTest.h"
#include "ChannelTest.h"
#include "PortLoopTest.h"
#include "progresstest.h"
#include "Settings.h"

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
    testRunner.addTest(new ProgressTest);
    testRunner.addTest(new CalibrationSourceTest);
    testRunner.addTest(new ChannelTest(CONNECTION_TYPE, INSTRUMENT_ADDRESS));
    testRunner.addTest(new CorrectionsTest(CONNECTION_TYPE, INSTRUMENT_ADDRESS));
    testRunner.addTest(new CalculateTest(CONNECTION_TYPE, INSTRUMENT_ADDRESS));

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}

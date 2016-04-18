

// Tests
#include "CalculateTest.h"

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
    testRunner.addTest(new CalculateTest);

    qDebug() << "Global result: " << (testRunner.runTests() ? "PASS" : "FAIL");
    return 0;
}

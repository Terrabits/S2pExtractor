#include "progresstest.h"

#include "progress.h"

using namespace RsaToolbox;

ProgressTest::ProgressTest(QObject *parent) : TestClass(parent)
{

}

void ProgressTest::generalTest() {
    // Expected percents:
    // 25%/major increment
    //  5%/minor increment.
    Progress progress(4,4);
    // major = 0, minor = 0
    QCOMPARE(progress.percent(), uint(0));

    for (int minor = 1; minor <= 4; minor++) {
        progress.minorIncrement();
        QCOMPARE(progress.percent(), uint(5 * minor));
    }
    for (int major = 1; major <= 4; major++) {
        progress.majorIncrement();
        QCOMPARE(progress.percent(), uint(25 * major));
        for (int minor = 1; minor <= 4; minor++) {
            progress.minorIncrement();
            QCOMPARE(progress.percent(), uint(25*major + 5*minor));
        }
    }
}

void ProgressTest::minorOverincrementTest() {
    Progress progress(10,9);
    QCOMPARE(progress.percent(), uint(0));
    for (int i = 0; i < 20; i++) {
        progress.minorIncrement();
    }
    QCOMPARE(progress.percent(), uint(9));
}

void ProgressTest::majorOverincrementTest() {
    Progress progress(10, 4);
    for (int i = 0; i < 20; i++) {
        progress.majorIncrement();
    }
    QCOMPARE(progress.percent(), uint(100));
}

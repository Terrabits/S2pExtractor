#ifndef CALIBRATIONSOURCETEST_H
#define CALIBRATIONSOURCETEST_H


// RsaToolbox
#include <TestClass.h>

// Qt
#include <QObject>

class CalibrationSourceTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    CalibrationSourceTest(QObject *parent = 0);
    ~CalibrationSourceTest();

private slots:
    void noSource();
    void channel();
    void calGroup();
    void switchSource();
    void assignmentOperator();
};

#endif // CALIBRATIONSOURCETEST_H

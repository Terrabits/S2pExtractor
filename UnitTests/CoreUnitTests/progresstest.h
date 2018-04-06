#ifndef PROGRESSTEST_H
#define PROGRESSTEST_H

#include <TestClass.h>
#include <QObject>

class ProgressTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    explicit ProgressTest(QObject *parent = 0);

private slots:
    void generalTest();
    void minorOverincrementTest();
    void majorOverincrementTest();
};

#endif // PROGRESSTEST_H

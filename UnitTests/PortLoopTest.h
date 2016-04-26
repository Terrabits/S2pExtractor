#ifndef PORTLOOPTEST_H
#define PORTLOOPTEST_H

// RsaToolbox
#include <TestClass.h>

// Qt
#include <QObject>

class PortLoopTest : public RsaToolbox::TestClass
{
    Q_OBJECT
public:
    explicit PortLoopTest(QObject *parent = 0);
    ~PortLoopTest();

private slots:
    void noPorts();
    void oneOfTwoPortsProcessed();
    void twoPortsProcessed();
    void manyPortsProcessed();
    void cannotProcessPorts();
};

#endif // PORTLOOPTEST_H

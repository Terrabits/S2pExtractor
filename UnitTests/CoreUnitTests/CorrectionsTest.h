#ifndef CORRECTIONSTEST_H
#define CORRECTIONSTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QObject>
#include <QStringList>


class CorrectionsTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    CorrectionsTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent = 0);
    ~CorrectionsTest();

private slots:
    // Once
    virtual void initTestCase();
    virtual void cleanupTestCase();

    // Source test
    void channelSource();
    void calGroupSource();

    // Vna ports
    void ports12();
    void ports1to4();
    void ports1to4Osm();
    void port1Osm();

    // Switch matrix ports
    void ports12SwMat();
    void ports1to4SwMat();
    void ports1to4SwMatOsm();
    void port1SwMatOsm();

private:
    QDir _calGroupDir;
    QStringList _calGroups;

    void addSwitchMatrix();
    void removeSwitchMatrices();
};

#endif // CORRECTIONSTEST_H

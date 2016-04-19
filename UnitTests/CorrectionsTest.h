#ifndef CORRECTIONSTEST_H
#define CORRECTIONSTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QObject>


class CorrectionsTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit CorrectionsTest(QObject *parent = 0);
    ~CorrectionsTest();

private slots:
    void ports12();
    void ports1to4();
    void ports1to4Osm();
    void port1Osm();

    void ports12SwMat();
    void ports1to4SwMat();
    void ports1to4SwMatOsm();
    void port1SwMatOsm();

private:
    QDir _calGroupDir;

    void addSwitchMatrix();
    void removeSwitchMatrices();
};

#endif // CORRECTIONSTEST_H

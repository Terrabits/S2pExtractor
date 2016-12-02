#ifndef CALCULATETEST_H
#define CALCULATETEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QStringList>
#include <QDir>
#include <QObject>


class CalculateTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit CalculateTest(QObject *parent = 0);
    ~CalculateTest();

private slots:
    // Once
    virtual void initTestCase();
    virtual void cleanupTestCase();

    // Tests
    void isReady_data();
    void isReady();

    void uosmCalculations();
    void osmCalculations();

private:
    QDir _calGroupDir;
    QStringList _calGroups;

    QDir _resultsDir;
};

#endif // CALCULATETEST_H

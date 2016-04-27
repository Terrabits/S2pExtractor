#ifndef CALCULATETEST_H
#define CALCULATETEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QDir>
#include <QObject>


class CalculateTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    explicit CalculateTest(QObject *parent = 0);
    ~CalculateTest();

private slots:
    void uosmCalculations();
    void osmCalculations();

};

#endif // CALCULATETEST_H

#ifndef CHANNELTEST_H
#define CHANNELTEST_H


// RsaToolbox
#include <VnaTestClass.h>

// Qt
#include <QDir>
#include <QStringList>


class ChannelTest : public RsaToolbox::VnaTestClass
{
    Q_OBJECT
public:
    ChannelTest(RsaToolbox::ConnectionType type, const QString &address, QObject *parent = 0);
    ~ChannelTest();

private slots:
    // Once
    virtual void initTestCase();
    virtual void cleanupTestCase();

    // Once per test
    virtual void init();

    void isReady_data();
    void isReady();

private:
    QDir _calGroupDir;
    QStringList _calGroups;

};

#endif // CHANNELTEST_H

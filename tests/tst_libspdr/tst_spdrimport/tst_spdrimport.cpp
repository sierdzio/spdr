#include "spdrglobal.h"
#include "SpdrImport"

#include <QObject>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

class TstSpdrImport : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
    void testSignals();
    void testFormatSetting();
};

void TstSpdrImport::testDefaults()
{
    SpdrImport testObject;
    QCOMPARE(testObject.format(), QString());
}

void TstSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setFormat("./");
    QCOMPARE(testObject.format(), QString("./"));

    QString propertyTestValue("./MM/");
    testObject.setProperty("format", propertyTestValue);
    QCOMPARE(testObject.format(), propertyTestValue);
    QCOMPARE(testObject.property("format").toString(), propertyTestValue);
}

void TstSpdrImport::testSignals()
{
    Spdr::registerMetatypes();
    SpdrImport testObject;
    QSignalSpy spy(&testObject, SIGNAL(formatChanged(QString)));

    testObject.setFormat("aaa");
    testObject.setProperty("format", "bbb");

    QCOMPARE(spy.count(), 2);
}

void TstSpdrImport::testFormatSetting()
{
    SpdrImport testObject;
    testObject.setLogLevel(Spdr::NoLogging);
    QString testFormat("./<badTag/here");
    bool result = true;

    // Setting the format should fail here: date tag is not closed
    result = testObject.setFormat(testFormat);
    QCOMPARE(testObject.format(), QString());
    QCOMPARE(result, false);

    // Setting the format should fail here: tag is closed before opening
    testFormat = "./>MM</";
    result = true;
    result = testObject.setFormat(testFormat);
    QCOMPARE(testObject.format(), QString());
    QCOMPARE(result, false);

    // Setting the format should be successful: mixing Windows and Unix style
    // path is allowed
    testFormat = "./<yyyy>\\<MM>/";
    result = false;
    result = testObject.setFormat(testFormat);
    QCOMPARE(testObject.format(), testFormat);
    QCOMPARE(result, true);

    // Setting the format should fail here: tag is opened twice but closed once
    testObject.setFormat(QString());
    testFormat = "./<<MM>/";
    result = true;
    result = testObject.setFormat(testFormat);
    QCOMPARE(testObject.format(), QString());
    QCOMPARE(result, false);
}

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


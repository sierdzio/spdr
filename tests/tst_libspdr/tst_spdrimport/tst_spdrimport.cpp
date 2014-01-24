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
};

void TstSpdrImport::testDefaults()
{
    SpdrImport testObject;
    QCOMPARE(testObject.format(), QString());
}

void TstSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setFormat("Spdr::Move");
    QCOMPARE(testObject.format(), QString("Spdr::Move"));

    QString propertyTestValue("another value");
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

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


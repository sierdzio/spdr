#include "spdrglobal.h"
#include "SpdrImport"

#include <QObject>
#include <QDebug>
#include <QtTest/QtTest>

class TstSpdrImport : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
};

void TstSpdrImport::testDefaults()
{
    //qDebug() << "Testing defaults for SpdrBase";
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

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


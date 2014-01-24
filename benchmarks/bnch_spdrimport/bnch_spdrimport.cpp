#include "spdrglobal.h"
#include "SpdrImport"

#include <QObject>
#include <QDebug>
#include <QtTest/QtTest>

class BnchSpdrImport : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
};

void BnchSpdrImport::testDefaults()
{
    SpdrImport testObject;
    QCOMPARE(testObject.format(), QString());
}

void BnchSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setFormat("Spdr::Move");
    QCOMPARE(testObject.format(), QString("Spdr::Move"));

    QString propertyTestValue("another value");
    testObject.setProperty("format", propertyTestValue);
    QCOMPARE(testObject.format(), propertyTestValue);
    QCOMPARE(testObject.property("format").toString(), propertyTestValue);
}

QTEST_MAIN(BnchSpdrImport)
#include "bnch_spdrimport.moc"


#include "spdrglobal.h"
#include "SpdrSynchronize"

#include <QObject>
#include <QDebug>
#include <QtTest/QtTest>

class TstSpdrSynchronize : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
};

void TstSpdrSynchronize::testDefaults()
{
    SpdrSynchronize testObject;
    QCOMPARE(testObject.options(), SpdrSynchronize::None);
    QCOMPARE(testObject.split(), 0);
}

void TstSpdrSynchronize::testSetters()
{
    SpdrSynchronize testObject;
    SpdrSynchronize::SynchronizationOptions options = SpdrSynchronize::Cache
            | SpdrSynchronize::DeepSearch;

    testObject.setOptions(options);
    QCOMPARE(testObject.options(), options);

    int propertyTestValue = 8;
    testObject.setSplit(propertyTestValue);
    QCOMPARE(testObject.split(), propertyTestValue);

    propertyTestValue = 3;
    testObject.setProperty("split", propertyTestValue);
    QCOMPARE(testObject.split(), propertyTestValue);
    QCOMPARE(testObject.property("split").toInt(), propertyTestValue);
}

QTEST_MAIN(TstSpdrSynchronize)
#include "tst_spdrsynchronize.moc"

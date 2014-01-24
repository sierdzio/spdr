#include "spdrglobal.h"
#include "SpdrSynchronize"

#include <QList>
#include <QVariant>
#include <QObject>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>

class TstSpdrSynchronize : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
    void testSignals();
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

void TstSpdrSynchronize::testSignals()
{
    SpdrSynchronize::registerMetatypes();
    SpdrSynchronize testObject;
    QSignalSpy spyOptions(&testObject, SIGNAL(optionsChanged(SpdrSynchronize::SynchronizationOptions)));
    QSignalSpy spySplit(&testObject, SIGNAL(splitChanged(int)));

    testObject.setOptions(SpdrSynchronize::Bidirectional);
    QCOMPARE(spyOptions.count(), 1);

    testObject.setSplit(5);
    testObject.setProperty("split", 7);
    QCOMPARE(spySplit.count(), 2);
    QList<QVariant> arguments = spySplit.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 7);
}

QTEST_MAIN(TstSpdrSynchronize)
#include "tst_spdrsynchronize.moc"


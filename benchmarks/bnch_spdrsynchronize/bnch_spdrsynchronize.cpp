#include "spdrglobal.h"
#include "SpdrSynchronize"

#include <QObject>
#include <QDebug>
#include <QTest>

class BnchSpdrSynchronize : public QObject
{
    Q_OBJECT

private slots:
    void testSetters();
    void benchmarkSplit_data();
    void benchmarkSplit();
};

void BnchSpdrSynchronize::testSetters()
{
    SpdrSynchronize testObject;

    int propertyTestValue = 3;
    QBENCHMARK {
        testObject.setProperty("split", propertyTestValue);
    }

    QCOMPARE(testObject.split(), propertyTestValue);
    QCOMPARE(testObject.property("split").toInt(), propertyTestValue);
}

void BnchSpdrSynchronize::benchmarkSplit_data()
{
    QTest::addColumn<bool>("splitCompare");
    QTest::newRow("compare using Q_PROPERTY") << true;
    QTest::newRow("compare using C++") << false;
}

void BnchSpdrSynchronize::benchmarkSplit()
{
    QFETCH(bool, splitCompare);
    SpdrSynchronize testObject;

    if (splitCompare) {
        QBENCHMARK {
            testObject.setProperty("split", 5);
        }
    } else {
        QBENCHMARK {
            testObject.setSplit(5);
        }
    }
}

QTEST_MAIN(BnchSpdrSynchronize)
#include "bnch_spdrsynchronize.moc"


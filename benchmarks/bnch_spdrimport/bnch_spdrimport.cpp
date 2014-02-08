#include "spdrglobal.h"
#include "SpdrImport"

#include <QObject>
#include <QDebug>
#include <QTest>

/*!
  \addtogroup benchmarks Spdr Benchmarks

  \brief Spdr comes bundled with a set of benchmarks that (should) show the differences
  between using various import/ synchronization settings.
  */

/*!
  \ingroup benchmarks

  Benchmark for SpdrImport. Currently it is more than basic - it does not test
  anything much.
 */
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
    QCOMPARE(testObject.outputPath(), QString());
}

void BnchSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setOutputPath("Spdr::Move");
    QCOMPARE(testObject.outputPath(), QString("Spdr::Move"));

//    QString propertyTestValue("another value");
//    testObject.setProperty("format", propertyTestValue);
//    QCOMPARE(testObject.format(), propertyTestValue);
//    QCOMPARE(testObject.property("format").toString(), propertyTestValue);
}

QTEST_MAIN(BnchSpdrImport)
#include "bnch_spdrimport.moc"


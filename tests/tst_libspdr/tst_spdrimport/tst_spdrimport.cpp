#include "spdrglobal.h"
#include "SpdrImport"

#include <QString>
#include <QStringList>
#include <QObject>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>
#include <QFile>
#include <QDir>
#include <QDate>

class TstSpdrImport : public QObject
{
    Q_OBJECT

private slots:
    void testDefaults();
    void testSetters();
    void testSignals();
    void testFormatSetting();
    void testBasicImporting();
};

void TstSpdrImport::testDefaults()
{
    SpdrImport testObject;
    QCOMPARE(testObject.outputPath(), QString());
}

void TstSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setOutputPath("./");
    QCOMPARE(testObject.outputPath(), QString("."));

    // TODO: test for property manipulation of output path
    /*
    QString propertyTestValue("./MM/");
    testObject.setProperty("format", propertyTestValue);
    QCOMPARE(testObject.format(), propertyTestValue);
    QCOMPARE(testObject.property("format").toString(), propertyTestValue);
    */
}

void TstSpdrImport::testSignals()
{
    Spdr::registerMetatypes();
    SpdrImport testObject;
    QSignalSpy spy(&testObject, SIGNAL(outputPathChanged(QString)));

    testObject.setOutputPath("aaa");
    //testObject.setProperty("format", "bbb");

    QCOMPARE(spy.count(), 1);
}

void TstSpdrImport::testFormatSetting()
{
    SpdrImport testObject;
    testObject.setLogLevel(Spdr::NoLogging);
    QString testFormat("./<badTag/here");
    bool result = true;

    // Setting the format should fail here: date tag is not closed
    result = testObject.setOutputPath(testFormat);
    QCOMPARE(testObject.outputPath(), QString());
    QCOMPARE(result, false);

    // Setting the format should fail here: tag is closed before opening
    testFormat = "./>MM<";
    result = true;
    result = testObject.setOutputPath(testFormat);
    QCOMPARE(testObject.outputPath(), QString());
    QCOMPARE(result, false);

    // Setting the format should be successful: mixing Windows and Unix style
    // path is allowed
    testFormat = "./<yyyy>\\<MM>";
    result = false;
    result = testObject.setOutputPath(testFormat);
    QCOMPARE(testObject.outputPath(), testFormat);
    QCOMPARE(result, true);

    // Setting the format should fail here: tag is opened twice but closed once
    testFormat = "./<<MM>";
    result = true;
    result = testObject.setOutputPath(testFormat);
    QCOMPARE(testObject.outputPath(), QString());
    QCOMPARE(result, false);
}

/*!
  WARNING! This test will fail if it so happens, that the date (year or month)
  changes during it's execution. This is extremely unlikely, so don't worry.
 */
void TstSpdrImport::testBasicImporting()
{
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();
    int numberOfFiles = 15;
    QDir().mkpath(testDataPath + QLatin1String("/subdir"));
    for (int i = 0; i < numberOfFiles; i++) {
        QString filename(QString("file%1.txt").arg(QString::number(i)));

        if (i % 2) {
            filename = "subdir/" + filename;
        }

        filename = testDataPath + "/" + filename;

        QFile file(filename);

        if (!file.open(QFile::Text | QFile::WriteOnly)) {
            continue;
        }

        QString fileContent("Content of file number: ");
        fileContent += QString::number(i);
        fileContent += ". Random data: ";
        fileContent += QString::number(qrand());
        file.write(fileContent.toUtf8());
        file.close();
    }

    SpdrImport testObject;
    testObject.setLogLevel(Spdr::OnlyErrors);
    testObject.setSimulate(true);
    testObject.setInputPath(testDataPath);
    testObject.setOutputPath(testDataPath + "/testOutput/<yyyy>/<MM>/");
    QCOMPARE(testObject.import(), true);

    // After simulation: no actual data should be copied
    QDir testOutputDir(testDataPath + QLatin1String("/testOutput"));
    QCOMPARE(testOutputDir.exists(), false);

    testObject.setSimulate(false);
    QCOMPARE(testObject.import(), true);
    QCOMPARE(testOutputDir.exists(), true);

    QDate currentDate(QDate::currentDate());
    QCOMPARE(testOutputDir.cd(currentDate.toString("yyyy")), true);
    QCOMPARE(testOutputDir.cd(currentDate.toString("MM")), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::NoDotAndDotDot).count(), numberOfFiles);

    QDir(testDataPath).removeRecursively();
}

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


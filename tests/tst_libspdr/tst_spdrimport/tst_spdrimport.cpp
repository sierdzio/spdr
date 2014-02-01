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
    void testStarSubstitutionImporting();

private:
    void createTestFiles(const QString &filePath, bool includeSubdir = false, int numberOfFiles = 5);
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

    QString testInputPath(testDataPath + QLatin1String("/testInput"));
    QString testOutputPath(testDataPath + QLatin1String("/testOutput"));
    QDir().mkpath(testInputPath);
    QDir().mkpath(testOutputPath);

    int numberOfFiles = 15;
    createTestFiles(testInputPath, true,  15);

    SpdrImport testObject;
    testObject.setLogLevel(Spdr::OnlyErrors);
    testObject.setSimulate(true);
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath + "/<yyyy>/<MM>/");
    QCOMPARE(testObject.import(), true);

    // After simulation: no actual data should be copied
    QDir testOutputDir(testOutputPath);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty(), true);

    testObject.setSimulate(false);
    QCOMPARE(testObject.import(), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty(), false);

    QDate currentDate(QDate::currentDate());
    QCOMPARE(testOutputDir.cd(currentDate.toString("yyyy")), true);
    QCOMPARE(testOutputDir.cd(currentDate.toString("MM")), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::NoDotAndDotDot).count(), numberOfFiles);

    QDir(testDataPath).removeRecursively();
}

void TstSpdrImport::testStarSubstitutionImporting()
{
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();

    QString testInputPath(testDataPath + QLatin1String("/testInput"));
    QString testOutputPath(testDataPath + QLatin1String("/testOutput"));
    QDir().mkpath(testInputPath);
    QDir().mkpath(testOutputPath);

    // Populate output path with folders that should match star expressions
    QDate currentDate(QDate::currentDate());
    QDir().mkpath(testOutputPath + QLatin1String("/") + currentDate.toString("yyyy")
                  + QLatin1String("/") + currentDate.toString("MM")
                  + QLatin1String(" - test text"));

    int numberOfFiles = 5;
    createTestFiles(testInputPath, true, numberOfFiles);

    SpdrImport testObject;
    testObject.setLogLevel(Spdr::OnlyErrors);
    testObject.setSimulate(true);
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath + "/<yyyy>/<MM>*/");
    QCOMPARE(testObject.import(), true);

    // After simulation: no actual data should be copied
    QDir testOutputDir(testOutputPath);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).count(), 1);

    testObject.setSimulate(false);
    QCOMPARE(testObject.import(), true);
    QCOMPARE(testOutputDir.exists(), true);

    QCOMPARE(testOutputDir.cd(currentDate.toString("yyyy")), true);
    QCOMPARE(testOutputDir.cd(currentDate.toString("MM") + " - test text"), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::NoDotAndDotDot).count(), numberOfFiles);

    QDir(testDataPath).removeRecursively();
}

void TstSpdrImport::createTestFiles(const QString &filePath, bool includeSubdir, int numberOfFiles)
{
    QDir().mkpath(filePath + "/subdir");

    for (int i = 0; i < numberOfFiles; i++) {
        QString filename(QString("file%1.txt").arg(QString::number(i)));

        if (includeSubdir && (i % 2)) {
            filename = filePath + "/subdir/" + filename;
        } else {
            filename = filePath + "/" + filename;
        }

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
}

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


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

/*!
  \ingroup tests

  Test for SpdrImport. It verifies the operation of all SpdrImport properties,
  and makes sure that importing works correctly.
 */
class TstSpdrImport : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDefaults();
    void testSetters();
    void testSignals();
    void testFormatSetting();
    void testBasicImporting();
    void testStarSubstitutionImporting();

private:
    int createTestFiles(const QString &basePath, bool includeSubdir = false);

    QString logFilePath;
};

void TstSpdrImport::initTestCase()
{
    logFilePath = "testLog.txt";
    QFile::remove(logFilePath);
}

void TstSpdrImport::testDefaults()
{
    SpdrImport testObject;
    QCOMPARE(testObject.outputPath(), QString());
    QCOMPARE(testObject.copyMode(), SpdrImport::Copy);
}

void TstSpdrImport::testSetters()
{
    SpdrImport testObject;

    testObject.setOutputPath("./");
    QCOMPARE(testObject.outputPath(), QString("."));

    QString pathTester(QCoreApplication::instance()->applicationDirPath());
    testObject.setOutputPath(pathTester);
    QCOMPARE(testObject.outputPath(), QString(pathTester));

    pathTester = pathTester + "/<yyyy>/<MM>";
    testObject.setOutputPath(pathTester);
    QCOMPARE(testObject.outputPath(), QString(pathTester));

    // SpdrImport accepts invalid paths because it will try to create
    // the whole output path while importing.
    QString newPathTester("/some/path");
    testObject.setOutputPath(newPathTester);
    QCOMPARE(testObject.outputPath(), QString(newPathTester));

    testObject.setCopyMode(SpdrImport::Move);
    QCOMPARE(testObject.copyMode(), SpdrImport::Move);

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
    testObject.registerMetatypes();
    QSignalSpy spy(&testObject, SIGNAL(outputPathChanged(QString)));
    QSignalSpy spyCopyMode(&testObject, SIGNAL(copyModeChanged(SpdrImport::CopyMode)));

    testObject.setOutputPath("aaa");
    //testObject.setProperty("format", "bbb");
    testObject.setCopyMode(SpdrImport::Move);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyCopyMode.count(), 1);
}

void TstSpdrImport::testFormatSetting()
{
    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
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
    //QDir().mkpath(testOutputPath);

    int numberOfFiles = createTestFiles(testInputPath, true);

    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
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

    int numberOfFiles = createTestFiles(testInputPath, true);

    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
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

int TstSpdrImport::createTestFiles(const QString &basePath, bool includeSubdir)
{
    QDir().mkpath(basePath + "/subdir");
    int numberOfFiles = 15;

    for (int i = 0; i < numberOfFiles; i++) {
        QString filename(QString("file%1.txt").arg(QString::number(i)));

        if (includeSubdir && (i % 2)) {
            filename = basePath + "/subdir/" + filename;
        } else {
            filename = basePath + "/" + filename;
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

    return numberOfFiles;
}

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"


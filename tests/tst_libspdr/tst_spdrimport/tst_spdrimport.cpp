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

#include "../testhelpers.h"

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
    void testSuffixCaseSensitivity();

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
    TestHelpers::prepareTestDataDir(true);

    int numberOfFiles = createTestFiles(TestHelpers::testInputPath, true);

    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setSimulate(true);
    testObject.setInputPath(TestHelpers::testInputPath);
    testObject.setOutputPath(TestHelpers::testOutputPath + "/<yyyy>/<MM>/");
    QCOMPARE(testObject.import(), true);

    // After simulation: no actual data should be copied
    QDir testOutputDir(TestHelpers::testOutputPath);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty(), true);

    testObject.setSimulate(false);
    QCOMPARE(testObject.import(), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).isEmpty(), false);

    QDate currentDate(QDate::currentDate());
    QCOMPARE(testOutputDir.cd(currentDate.toString("yyyy")), true);
    QCOMPARE(testOutputDir.cd(currentDate.toString("MM")), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::NoDotAndDotDot).count(), numberOfFiles);

    TestHelpers::cleanUpTestDataDir();
}

void TstSpdrImport::testStarSubstitutionImporting()
{
    TestHelpers::prepareTestDataDir(true);

    // Populate output path with folders that should match star expressions
    QDate currentDate(QDate::currentDate());
    QDir().mkpath(TestHelpers::testOutputPath + QLatin1String("/") + currentDate.toString("yyyy")
                  + QLatin1String("/") + currentDate.toString("MM")
                  + QLatin1String(" - test text"));

    int numberOfFiles = createTestFiles(TestHelpers::testInputPath, true);

    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setSimulate(true);
    testObject.setInputPath(TestHelpers::testInputPath);
    testObject.setOutputPath(TestHelpers::testOutputPath + "/<yyyy>/<MM>*/");
    QCOMPARE(testObject.import(), true);

    // After simulation: no actual data should be copied
    QDir testOutputDir(TestHelpers::testOutputPath);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot).count(), 1);

    testObject.setSimulate(false);
    QCOMPARE(testObject.import(), true);
    QCOMPARE(testOutputDir.exists(), true);

    QCOMPARE(testOutputDir.cd(currentDate.toString("yyyy")), true);
    QCOMPARE(testOutputDir.cd(currentDate.toString("MM") + " - test text"), true);
    QCOMPARE(testOutputDir.entryList(QDir::Files | QDir::NoDotAndDotDot).count(), numberOfFiles);

    TestHelpers::cleanUpTestDataDir();
}

void TstSpdrImport::testSuffixCaseSensitivity()
{
    TestHelpers::prepareTestDataDir(true);

    // Write one file, copy to output, then rename the suffix to test.
    TestHelpers::writeTestFile(1, TestHelpers::testInputPath,
                               TestHelpers::generateFileData(1));
    QString outputPath(TestHelpers::testOutputPath + "/" + QString::number(QDate::currentDate().year()));
    QDir().mkpath(outputPath);

    QFile::copy(TestHelpers::testInputPath + "/file1.txt", outputPath + "/file1.txt");
    QFile::rename(TestHelpers::testInputPath + "/file1.txt", TestHelpers::testInputPath + "/file1.TxT");

    // Test with case insensitivity
    SpdrImport testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setSimulate(false);
    testObject.setSuffixCaseSensitive(false);
    testObject.setInputPath(TestHelpers::testInputPath);
    testObject.setOutputPath(TestHelpers::testOutputPath + "/<yyyy>/");
    QCOMPARE(testObject.import(), true);

    // Observe the results. There should be just one, original file.
    QStringList outFiles(QDir(outputPath + "/").entryList(QDir::NoDotAndDotDot | QDir::Files));
    QCOMPARE(outFiles.length(), 1);
    QCOMPARE(outFiles.at(0), QString("file1.txt"));

    TestHelpers::cleanUpTestDataDir();
}

int TstSpdrImport::createTestFiles(const QString &basePath, bool includeSubdir)
{
    QDir().mkpath(basePath + "/subdir");
    int numberOfFiles = 15;

    for (int i = 0; i < numberOfFiles; i++) {
        if (includeSubdir && (i % 2)) {
            TestHelpers::writeTestFile(i, basePath + "/subdir/",
                                       TestHelpers::generateFileData(i));
        } else {
            TestHelpers::writeTestFile(i, basePath + "/",
                                       TestHelpers::generateFileData(i));
        }
    }

    return numberOfFiles;
}

QTEST_MAIN(TstSpdrImport)
#include "tst_spdrimport.moc"

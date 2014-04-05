#include "spdrglobal.h"
#include "SpdrSynchronize"

#include <QList>
#include <QVariant>
#include <QObject>
#include <QDebug>
#include <QTest>
#include <QSignalSpy>
#include <QDir>
#include <QFile>

// TODO: test UpdateMode properly
// TODO: test copied files in output
// TODO: test modified files
// TODO: expand tests to really analyse the output directory for correctness

/*!
  \ingroup tests

  Test for SpdrSynchronize. It verifies the operation of all SpdrSynchronize
  properties and makes sure the synchronization works correctly.

  It tests multiple synchronization scenarios, and thus can take a bit longer
  than the rest.
 */
class TstSpdrSynchronize : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDefaults();
    void testSetters();
    void testSignals();
    void testSimpleSynchronization();
    void testAdvancedSynchronization();
    void testDeepAdvancedSynchronization();

private:
    int createTestFiles(const QString &basePath, bool simplified);

    QString logFilePath;
};

void TstSpdrSynchronize::initTestCase()
{
    logFilePath = "testLog.txt";
    QFile::remove(logFilePath);
}

void TstSpdrSynchronize::testDefaults()
{
    SpdrSynchronize testObject;
    QCOMPARE(testObject.options(), SpdrSynchronize::RemoveEmptyDirectories
             | SpdrSynchronize::RemoveMissingFiles);
}

void TstSpdrSynchronize::testSetters()
{
    SpdrSynchronize testObject;
    SpdrSynchronize::SynchronizationOptions options = SpdrSynchronize::DeepSearch;

    testObject.setOptions(options);
    QCOMPARE(testObject.options(), options);
}

void TstSpdrSynchronize::testSignals()
{
    SpdrSynchronize::registerMetatypes();
    SpdrSynchronize testObject;
    QSignalSpy spyOptions(&testObject, SIGNAL(optionsChanged(SpdrSynchronize::SynchronizationOptions)));

    testObject.setOptions(SpdrSynchronize::DeepSearch);
    QCOMPARE(spyOptions.count(), 1);
}

/*!
  A simple, simulated synchronization that tests only basic features, like
  copying the newer version of a file to output.
 */
void TstSpdrSynchronize::testSimpleSynchronization()
{
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();

    QString testInputPath(testDataPath + QLatin1String("/input"));
    QString testOutputPath(testDataPath + QLatin1String("/output"));

    /*int numberOfFiles =*/ createTestFiles(testDataPath, true);

    SpdrSynchronize testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setSimulate(true);
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath);
    QCOMPARE(testObject.synchronize(), true);

    QDir(testDataPath).removeRecursively();
}

/*!
  Tests more advanced features of SpdrSynchronize: removing missing files,
  removing empty directories, moving files about.
 */
void TstSpdrSynchronize::testAdvancedSynchronization()
{
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();

    QString testInputPath(testDataPath + QLatin1String("/input"));
    QString testOutputPath(testDataPath + QLatin1String("/output"));

    createTestFiles(testDataPath, false);

    SpdrSynchronize testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setUpdateMode(Spdr::Overwrite);
    testObject.setOptions(SpdrSynchronize::RemoveMissingFiles
                          | SpdrSynchronize::RemoveEmptyDirectories);
    testObject.setSimulate(false);
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath);
    QCOMPARE(testObject.synchronize(), true);

    QDir(testDataPath).removeRecursively();
}

/*!
  Tests more advanced features of SpdrSynchronize: removing missing files,
  removing empty directories, moving files about. This time using DeepSearch.
 */
void TstSpdrSynchronize::testDeepAdvancedSynchronization()
{
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();

    QString testInputPath(testDataPath + QLatin1String("/input"));
    QString testOutputPath(testDataPath + QLatin1String("/output"));

    createTestFiles(testDataPath, false);

    SpdrSynchronize testObject;
    testObject.setLogFile(logFilePath);
    testObject.setLogLevel(Spdr::Debug);
    testObject.setUpdateMode(Spdr::Overwrite);
    testObject.setOptions(SpdrSynchronize::DeepSearch
                          | SpdrSynchronize::RemoveMissingFiles
                          | SpdrSynchronize::RemoveEmptyDirectories);
    testObject.setSimulate(false);
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath);
    QCOMPARE(testObject.synchronize(), true);

    QDir(testDataPath).removeRecursively();
}

int TstSpdrSynchronize::createTestFiles(const QString &basePath, bool simplified)
{
    QString inputPath(basePath + "/input");
    QString outputPath(basePath + "/output");
    QDir().mkpath(inputPath);
    QDir().mkpath(outputPath);

    if (!simplified) {
        QDir().mkdir(outputPath + "/emptyDir1");
        QDir().mkdir(outputPath + "/emptyDir2");
    }

    int numberOfFiles = 20;

    for (int i = 0; i < numberOfFiles; i++) {
        QString filename(QString("file%1.txt").arg(QString::number(i)));
        QString inputFilePath = inputPath + "/" + filename;

        QFile file(inputFilePath);

        if (!file.open(QFile::Text | QFile::WriteOnly)) {
            continue;
        }

        QString fileContent("Content of file number: ");
        fileContent += QString::number(i);
        fileContent += ". Random data: ";
        fileContent += QString::number(qrand());
        file.write(fileContent.toUtf8());
        file.close();

        if (i != 0) { // file0.txt is no copied to output at all
            QString outputFilePath(outputPath + "/" + filename);

            if ((!simplified) && ((i == 1) || (i == 2) || (i == 3) || (i == 4) || (i == 5))) {
                if (i == 1) {
                    // Moved input file
                    QDir().mkpath(inputPath + "/moved");
                    QFile::copy(inputFilePath, outputFilePath);
                    QFile::rename(inputFilePath, inputPath + "/moved/renamedFile1.txt");
                } else if (i == 2) {
                    // Missing input file
                    QFile::copy(inputFilePath, outputFilePath);
                    QFile::remove(inputFilePath);
                } else if (i == 3) {
                    // Renamed (moved) output file
                    QFile::copy(inputFilePath, outputFilePath);
                    QFile::rename(outputFilePath, outputPath + "/anotherFile3.txt");
                } else if (i == 4) {
                    // Copied input file
                    QFile::copy(inputFilePath, outputFilePath);
                    QFile::copy(inputFilePath, inputPath + "/anotherFile4.txt");
                    QFile::copy(inputFilePath, inputPath + "/zanotherFile4.txt");
                } else if (i == 5) {
                    // Modified input file
                    QFile::copy(inputFilePath, outputFilePath);
                    QFile file(inputFilePath);

                    if (!file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
                        continue;
                    }

                    QString fileContent("Additional data for file. More random data: ");
                    fileContent += QString::number(qrand());
                    file.write(fileContent.toUtf8());
                    file.close();
                }
            } else {
                QFile::copy(inputFilePath, outputFilePath);
            }
        }
    }

    return numberOfFiles;
}

QTEST_MAIN(TstSpdrSynchronize)
#include "tst_spdrsynchronize.moc"


#include "spdrglobal.h"
#include "SpdrSynchronize"

#include <QObject>
#include <QDebug>
#include <QTest>
#include <QFile>
#include <QDir>

/*!
  \ingroup benchmarks

  Benchmark for SpdrSynchronize: it tests the time differences when synchronizing
  medium various amounts of files (currently: 150) with different settings.

  Particularly interesting are the differences between using Deep Search and
  normal search, and between running a simulation and a real synchronization.
 */
class BnchSpdrSynchronize : public QObject
{
    Q_OBJECT

private slots:
    void simpleSynchronizationBenchmark_data();
    void simpleSynchronizationBenchmark();

private:
    int createTestFiles(const QString &basePath, bool simplified);
};

void BnchSpdrSynchronize::simpleSynchronizationBenchmark_data()
{
    QTest::addColumn<bool>("simulate");
    QTest::addColumn<bool>("deep");
    QTest::newRow("Simulate shallow") << true << false;
    QTest::newRow("Simulate deep") << true << true;
    QTest::newRow("Normal shallow") << false << false;
    QTest::newRow("Normal deep") << false << true;
}

void BnchSpdrSynchronize::simpleSynchronizationBenchmark()
{
    QFETCH(bool, simulate);
    QFETCH(bool, deep);
    QString testDataPath("testData");
    QDir(testDataPath).removeRecursively();

    QString testInputPath(testDataPath + QLatin1String("/input"));
    QString testOutputPath(testDataPath + QLatin1String("/output"));

    createTestFiles(testDataPath, false);

    SpdrSynchronize testObject;
    testObject.setLogLevel(Spdr::Error);
    if (deep) {
        testObject.setOptions(SpdrSynchronize::RemoveMissingFiles
                              | SpdrSynchronize::RemoveEmptyDirectories
                              | SpdrSynchronize::DeepSearch);
    } else {
        testObject.setOptions(SpdrSynchronize::RemoveMissingFiles
                              | SpdrSynchronize::RemoveEmptyDirectories);
    }
    testObject.setInputPath(testInputPath);
    testObject.setOutputPath(testOutputPath);

    testObject.setSimulate(simulate);

    QBENCHMARK {
        QCOMPARE(testObject.synchronize(), true);
    }

    QDir(testDataPath).removeRecursively();
}

int BnchSpdrSynchronize::createTestFiles(const QString &basePath, bool simplified)
{
    QString inputPath(basePath + "/input");
    QString outputPath(basePath + "/output");
    QDir().mkpath(inputPath);
    QDir().mkpath(outputPath);

    if (!simplified) {
        QDir().mkdir(outputPath + "/emptyDir1");
        QDir().mkdir(outputPath + "/emptyDir2");
    }

    int numberOfFiles = 1500;

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

        for (int j = 0; j < (i / 2); ++j) {
            fileContent += QString::number(qrand());
        }

        file.write(fileContent.toUtf8());
        file.close();

        if (i != 0) {
            if ((!simplified) && ((i == 1) || (i == 2))) {
                if (i == 1) {
                    // Moved file
                    QDir().mkpath(inputPath + "/moved");
                    QFile::copy(inputFilePath, outputPath + "/" + filename);
                    QFile::rename(inputFilePath, inputPath + "/moved/renamedFile1.txt");
                } else if (i == 2) {
                    // Missing file
                    QFile::copy(inputFilePath, outputPath + "/" + filename);
                    QFile::remove(inputFilePath);
                }
            } else {
                QFile::copy(inputFilePath, outputPath + "/" + filename);
            }
        }
    }

    return numberOfFiles;
}

QTEST_MAIN(BnchSpdrSynchronize)
#include "bnch_spdrsynchronize.moc"


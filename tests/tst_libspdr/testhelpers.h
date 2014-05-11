#ifndef TESTHELPERS_H
#define TESTHELPERS_H

#include <QString>
#include <QDir>
#include <QFile>

class TestHelpers {
public:
    /*!
      Default test data dir.
      */
    static QString testDataPath;

    /*!
      Default test data dir.
      */
    static QString testInputPath;

    /*!
      Default test data dir.
      */
    static QString testOutputPath;

    /*!
      Cleans up and prepares test directory for usage.
      */
    static void prepareTestDataDir(bool includeInputAndOutput = false)
    {
        cleanUpTestDataDir();
        QDir().mkpath(testDataPath);

        if (includeInputAndOutput) {
            QDir().mkpath(testInputPath);
            QDir().mkpath(testOutputPath);
        }
    }

    /*!
      Cleans up test data directory.
      */
    static void cleanUpTestDataDir()
    {
        QDir(testDataPath).removeRecursively();
    }

    /*!
      Generates a semi-randomized string that can be used to fill in a file.

      \a fileNumber is written into the data.
     */
    static QString generateFileData(quint32 fileNumber)
    {
        QString result("Content of file number: ");
        result += QString::number(fileNumber);
        result += ". Random data: ";
        result += QString::number(qrand());
        return result;
    }

    /*!
      Writes the \a data to file named "fileX.txt", where X is \a fileNumber,
      and which will be written into \a directory.

      Optionally, you can use \a fileNameOverride to provide a custom file name.
     */
    static void writeTestFile(quint32 fileNumber, const QString &directory, const QString &data,
                              const QString &fileNameOverride = QString::null)
    {
        QString filename;

        if (fileNameOverride.isEmpty()) {
            filename = QString("file%1.txt").arg(fileNumber);
        } else {
            filename = fileNameOverride;
        }

        if (directory.endsWith('/') || directory.endsWith('\\')) {
            filename = directory + filename;
        } else {
            filename = directory + "/" + filename;
        }

        QFile file(filename);

        if (!file.open(QFile::Text | QFile::WriteOnly)) {
            return;
        }

        file.write(data.toUtf8());
        file.close();
    }
};

QString TestHelpers::testDataPath = "testData";
QString TestHelpers::testInputPath = TestHelpers::testDataPath + QLatin1String("/testInput");
QString TestHelpers::testOutputPath = TestHelpers::testDataPath + QLatin1String("/testOutput");

#endif // TESTHELPERS_H

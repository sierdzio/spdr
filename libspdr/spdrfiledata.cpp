#include "spdrfiledata.h"
#include "SpdrLog"

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QDir>

/*!
  \class SpdrFileData
  \ingroup libspdr
  \internal

  \brief Used to cache the information about a file.

  Can be used to compare two files
  and dump file info. This class is not exported in Spdr library, is intentionally
  kept lightweight (does not inherit any other classes) to allow it to be instantiated
  in large quantities without too much overhead.
 */

/*!
  Simple initializing constructor, useful as an easier equivalent to:
  SpdrFileData data;
  data.readFileData(filePath, relativePathBase, deepSearch, logger);
 */
SpdrFileData::SpdrFileData(const QString &filePath, const QString &relativePathBase, SearchDepth searchDepth, const SpdrLog *logger)
{
    readFileData(filePath, relativePathBase, searchDepth, logger);
}

/*!
  Compares to files using most of available data.

  Warning! To facilitate fast track comparison, this operator ignores
  the checksums! For real equality, see isEqual

  \sa isEqual
  */
bool SpdrFileData::operator ==(const SpdrFileData &other) const
{
    if ((isValid == other.isValid) && (name == other.name) && (path == other.path)
            && (absoluteFilePath == other.absoluteFilePath)
            //&& (checksumMd5 == other.checksumMd5) && (checksumSha == other.checksumSha)
            && (creationDate == other.creationDate) && (size == other.size)) {
        return true;
    }

    return false;
}

/*!
  Returns true if current file is equal to \a other. That means that the names,
  creation dates, checksums and sizes are all the same.

  \sa isMoved
  */
bool SpdrFileData::isEqual(const SpdrFileData &other) const
{
    if ((name == other.name) && (checksumMd5 == other.checksumMd5)
            && (checksumSha == other.checksumSha)
            && (size == other.size))
    {
        return true;
    }

    return false;
}

/*!
  Returns true if the current file and \a other are the same, but moved
  to some other place (and/ or renamed).

  \sa isEqual
  */
bool SpdrFileData::isMoved(const SpdrFileData &other) const
{
    if ((checksumMd5 == other.checksumMd5)
            && (checksumSha == other.checksumSha)
            && (size == other.size))
    {
        return true;
    }

    return false;
}

/*!
  Returns a nicely formatted information about the file.
  */
QString SpdrFileData::toString() const
{
    QString result("Name: " + name + "\n");
    result += "Relative path: " + path + "\n";
    result += "Absolute file path: " + absoluteFilePath + "\n";
    result += "MD5: " + checksumMd5 + "\n";
    result += "SHA: " + checksumSha + "\n";
    result += "Created: " + creationDate.toString("dd-MM-yyyy") + "\n";
    result += "Size: " + QString::number(size);
    return result;
}

/*!
  Populates the object with all required data. Returns true if all required operations
  were successful. Use \a filePath to specify the file. \a relativePathBase is
  used to help Spdr determine the absolute path to the file (in case \a filePath
  is relative). When \a deepSearch is set, Spdr will calculate Sha1 checksum in
  addition to Md5. When \a logger is present, this method will print out log
  information using that object.
 */
bool SpdrFileData::readFileData(const QString &filePath,
                                const QString &relativePathBase,
                                SearchDepth searchDepth, const SpdrLog *logger)
{
    //bool isLogging = (logger != 0);

    QFileInfo fileInfo(filePath);
    isValid = true;
    name = fileInfo.fileName();

    absoluteFilePath = fileInfo.absoluteFilePath();
    path = QDir(relativePathBase).relativeFilePath(absoluteFilePath);

    creationDate = fileInfo.created();
    size = fileInfo.size();

    if (!setSearchDepth(searchDepth, logger)) {
        return false;
    }

//    if (isLogging) {
//        if (logger->logLevel() == Spdr::Debug) {
//            logger->log(toString(), Spdr::Debug);
//        } else {
//            logger->log(QCoreApplication::translate("SpdrFileData", "DB: Successfully added file %1 to the database").arg(path), Spdr::ExcessiveLogging);
//        }
//    }

    return true;
}

bool SpdrFileData::setSearchDepth(SpdrFileData::SearchDepth searchDepth, const SpdrLog *logger)
{
    bool isLogging = (logger != 0);
    QString fileReadError;
    QString fileHashingError;

    if (isLogging) {
        fileReadError = QCoreApplication::translate("SpdrFileData", "File could not be opened for reading while attempting to create a hash! %1")
                .arg(path);

        fileHashingError = QCoreApplication::translate("SpdrFileData", "Could not create an %1 hash for file %2");
    }

    if (searchDepth == ShallowSearch || searchDepth == DeepSearch) {
        QFile fileMd5(absoluteFilePath);
        if (fileMd5.open(QFile::ReadOnly)) {
            QCryptographicHash md5(QCryptographicHash::Md5);

            if (md5.addData(&fileMd5)) {
                checksumMd5 = md5.result();
            } else {
                if (isLogging) {
                    logger->log(fileHashingError.arg("MD5").arg(path), Spdr::Critical);
                }

                isValid = false;
                return false;
            }
        } else {
            if (isLogging) {
                logger->log(fileReadError, Spdr::Critical);
            }

            isValid = false;
            return false;
        }

        fileMd5.close();
    }

    if (searchDepth == DeepSearch) {
        QFile fileSha(absoluteFilePath);
        if (fileSha.open(QFile::ReadOnly)) {
            QCryptographicHash sha(QCryptographicHash::Sha1);

            if (sha.addData(&fileSha)) {
                checksumSha = sha.result();
            } else if (isLogging) {
                logger->log(fileHashingError.arg("SHA").arg(path), Spdr::Error);
            }
        } else if (isLogging) {
            logger->log(fileReadError, Spdr::Critical);
        }

        fileSha.close();
    }

    if (searchDepth == NoChecksums) {
        checksumMd5.clear();
        checksumSha.clear();
        isValid = true;
    }

    return true;
}

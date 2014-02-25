#include "spdrfiledata.h"
#include "SpdrLog"

#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QDir>

SpdrFileData::SpdrFileData(const QString &filePath, const QString &relativePathBase, bool deepSearch, const SpdrLog *logger)
{
    readFileData(filePath, relativePathBase, deepSearch, logger);
}

/*!
  Compares to files using most of available data.
  */
bool SpdrFileData::operator ==(const SpdrFileData &other) const
{
    if ((isValid == other.isValid) && (name == other.name) && (path == other.path)
            && (absoluteFilePath == other.absoluteFilePath)
            && (checksumMd5 == other.checksumMd5) && (checksumSha == other.checksumSha)
            && (creationDate == other.creationDate) && (size == other.size)) {
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
  Returns true if current file is equal to \a other. That means that the names,
  creation dates, checksums and sizes are all the same.
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

bool SpdrFileData::readFileData(const QString &filePath,
                                const QString &relativePathBase,
                                bool deepSearch, const SpdrLog *logger)
{
    bool isLogging = (logger != 0);

    QFileInfo fileInfo(filePath);
    isValid = true;
    name = fileInfo.fileName();

    absoluteFilePath = fileInfo.absoluteFilePath();
    path = QDir(relativePathBase).relativeFilePath(absoluteFilePath);

    creationDate = fileInfo.created();
    size = fileInfo.size();

    QString fileReadError;
    QString fileHashingError;

    if (isLogging) {
        fileReadError = logger->tr("File could not be opened for reading while attempting to create a hash! %1")
                      .arg(path);

        fileHashingError = logger->tr("Could not create an %1 hash for file %2");
    }

    QFile fileMd5(filePath);
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

    if (deepSearch) {
        QFile fileSha(filePath);
        if (fileSha.open(QFile::ReadOnly)) {
            QCryptographicHash sha(QCryptographicHash::Sha1);

            if (sha.addData(&fileSha)) {
                checksumMd5 = sha.result();
            } else if (isLogging) {
                logger->log(fileHashingError.arg("SHA").arg(path), Spdr::Error);
            }
        } else if (isLogging) {
            logger->log(fileReadError, Spdr::Critical);
        }

        fileSha.close();
    }

    return true;
}

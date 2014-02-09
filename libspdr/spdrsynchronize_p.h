#ifndef SPDRSYNCHRONIZE_P_H
#define SPDRSYNCHRONIZE_P_H

#include "spdrsynchronize.h"

#include <QMultiHash>
#include <QByteArray>
#include <QString>
#include <QDateTime>

/*!
  Used to cache the information about a file.
 */
class SpdrFileData
{
public:
    bool isValid;
    QString name;
    QString path;
    QString absoluteFilePath;
    QByteArray checksumMd5;
    QByteArray checksumSha;
    QDateTime creationDate;
    qint64 size;

    bool operator ==(const SpdrFileData &other) const
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
    QString toString() const
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
    bool isEqual(const SpdrFileData &other)
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
    bool isMoved(const SpdrFileData &other)
    {
        if ((checksumMd5 == other.checksumMd5)
                && (checksumSha == other.checksumSha)
                && (size == other.size))
        {
            return true;
        }

        return false;
    }
};

class SpdrSynchronizePrivate
{
    Q_DECLARE_PUBLIC(SpdrSynchronize)

public:
    SpdrSynchronizePrivate(SpdrSynchronize *q) : q_ptr(q) {}
    SpdrSynchronize *q_ptr;

    bool readDirectoryFileData(const QString &directoryPath,
                               QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const;
    bool readFileData(const QString &filePath,
                      QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const;

    bool synchronizeDirectory(const QString &directoryPath,
                              QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const;
    bool synchronizeFile(const QString &filePath,
                         QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const;

    bool removeEmptyDirectories(const QString &rootDirectoryPath) const;
    bool removeEmptyDirectory(const QString &directoryPath) const;

    SpdrFileData getFileData(const QString &filePath) const;
    QString getRelativeFilePath(const QString &absoluteFilePath) const;

    SpdrSynchronize::SynchronizationOptions mOptions;
    uint mSplit;
};

#endif // SPDRSYNCHRONIZE_P_H

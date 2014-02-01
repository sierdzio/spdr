#ifndef SPDRSYNCHRONIZE_P_H
#define SPDRSYNCHRONIZE_P_H

#include "spdrsynchronize.h"

#include <QHash>
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
    QByteArray checksumMd5;
    QByteArray checksumSha;
    QDateTime creationDate;
    qint64 size;

    QString toString() const
    {
        QString result("Name: " + name);
        result += "Relative path: " + path;
        result += "MD5: " + checksumMd5;
        result += "SHA: " + checksumSha;
        result += "Created: " + creationDate.toString("dd-MM-yyyy");
        result += "Size: " + QString::number(size);
        return result;
    }

    bool isEqual(const SpdrFileData &other)
    {
        if ((name == other.name) && (checksumMd5 == other.checksumMd5)
                && (checksumSha == other.checksumSha)
                && (creationDate == other.creationDate)
                && (size == other.size))
        {
            return true;
        }

        return false;
    }

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

    bool readDirectoryFileData(const QString &directoryPath, QHash<QByteArray, SpdrFileData> *fileHashTable) const;
    bool readFileData(const QString &filePath, QHash<QByteArray, SpdrFileData> *fileHashTable) const;

    bool synchronizeDirectory(const QString &directoryPath, QHash<QByteArray, SpdrFileData> &fileHashTable) const;
    bool synchronizeFile(const QString &filePath, QHash<QByteArray, SpdrFileData> &fileHashTable) const;

    SpdrFileData getFileData(const QString &filePath) const;
    QString getRelativeFilePath(const QString &absoluteFilePath) const;

    SpdrSynchronize::SynchronizationOptions mOptions;
    uint mSplit;
};

#endif // SPDRSYNCHRONIZE_P_H

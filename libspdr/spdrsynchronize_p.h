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
    QString name;
    QString path;
    QByteArray checksumMd5;
    QByteArray checksumSha;
    QDateTime creationDate;
    qint64 size;
};

class SpdrSynchronizePrivate
{
    Q_DECLARE_PUBLIC(SpdrSynchronize)

public:
    SpdrSynchronizePrivate(SpdrSynchronize *q) : q_ptr(q) {}
    SpdrSynchronize *q_ptr;

    bool readDirectoryFileData(const QString &directoryPath, QHash<QByteArray, SpdrFileData> *fileHashTable) const;
    bool readFileData(const QString &filePath, QHash<QByteArray, SpdrFileData> *fileHashTable) const;

    SpdrSynchronize::SynchronizationOptions mOptions;
    uint mSplit;
};

#endif // SPDRSYNCHRONIZE_P_H

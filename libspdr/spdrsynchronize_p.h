#ifndef SPDRSYNCHRONIZE_P_H
#define SPDRSYNCHRONIZE_P_H

#include "spdrsynchronize.h"
#include "spdrfiledata.h"

#include <QMultiHash>
#include <QByteArray>
#include <QString>
#include <QDateTime>

typedef QMultiHash<QByteArray,SpdrFileData> FileDataTable;

/*!
  \internal
  */
class SpdrSynchronizePrivate
{
    Q_DECLARE_PUBLIC(SpdrSynchronize)

public:
    SpdrSynchronizePrivate(SpdrSynchronize *q) : q_ptr(q) {}
    SpdrSynchronize *q_ptr;

    bool readDirectoryFileData(const QString &directoryPath,
                               FileDataTable *fileHashTable) const;
    bool readFileData(const QString &filePath,
                      FileDataTable *fileHashTable) const;

    bool synchronizeDirectory(const QString &directoryPath,
                              FileDataTable *fileHashTable) const;
    bool synchronizeFile(const QString &filePath,
                         FileDataTable *fileHashTable) const;

    bool removeEmptyDirectories(const QString &rootDirectoryPath) const;
    bool removeEmptyDirectory(const QString &directoryPath) const;

    QString getRelativePathBase(const QString &absoluteFilePath) const;
    SpdrFileData::SearchDepth searchDepth() const;

    SpdrSynchronize::SynchronizationOptions mOptions;
    uint mSplit;
};

#endif // SPDRSYNCHRONIZE_P_H

#ifndef SPDRFILEDATA_H
#define SPDRFILEDATA_H

#include <QByteArray>
#include <QString>
#include <QDateTime>

class SpdrLog;

class SpdrFileData
{
public:
    /*!
      Specifies file data reading depth. This flag is used mostly internally.
      */
    enum SearchDepth {
        NoChecksums = 0,
        ShallowSearch = 1,
        OnlyMd5 = ShallowSearch,
        DeepSearch = 2
    };

    SpdrFileData(const QString &filePath, const QString &relativePathBase,
                 SearchDepth searchDepth = ShallowSearch, const SpdrLog *logger = 0);

    bool isValid;
    QString name;
    QString path;
    QString absoluteFilePath;
    QByteArray checksumMd5;
    QByteArray checksumSha;
    QDateTime creationDate;
    qint64 size;

    bool operator ==(const SpdrFileData &other) const;
    bool isEqual(const SpdrFileData &other, bool suffixCaseSensitive = true) const;
    bool isMoved(const SpdrFileData &other) const;
    bool readFileData(const QString &filePath, const QString &relativePathBase,
                      SearchDepth searchDepth = ShallowSearch, const SpdrLog *logger = 0);
    bool setSearchDepth(SearchDepth searchDepth, const SpdrLog *logger = 0);
    QString toString() const;
    bool isFileNameEqual(const QString &name, const QString &other,
                         bool suffixCaseSensitive = true) const;
};

#endif // SPDRFILEDATA_H

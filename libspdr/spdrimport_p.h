#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"
#include <QRegularExpression>

class QChar;
class QString;

/*!
  \ingroup libspdr
  \internal
  */
class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    bool performFileOperation(const QString &inputFile, const QString &outputFile) const;
    bool areFilesTheSame(const QString &input, const QString &output) const;

    bool importDirectory(const QString &inputDirectoryPath) const;
    bool importFile(const QString &filePath) const;
    QString getOutputFilePath(const QString &inputFilePath) const;
    QString substituteStarsInPath(const QString &outputFilePath) const;

    bool checkFormat(const QString &format) const;
    int countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount) const;

    QRegularExpression mPathSeparatorRegularExpression;
    SpdrImport::CopyMode mCopyMode;
};

#endif // SPDRIMPORT_P_H

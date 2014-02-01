#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"

#include <QString>

class QChar;

class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    bool importDirectory(const QString &inputDirectoryPath) const;
    bool importFile(const QString &filePath) const;
    QString getOutputFilePath(const QString &inputFilePath) const;
    QString substituteStarsInPath(const QString &outputFilePath) const;
    QString getOperationStatusFromBool(bool status) const;

    bool checkFormat(const QString &format) const;
    int countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount) const;

    QString mPathSeparatorRegularExpression;
};

#endif // SPDRIMPORT_P_H

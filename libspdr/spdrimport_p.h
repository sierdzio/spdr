#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"

#include <QChar>
#include <QString>

class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) { mPathSeparatorRegularExpression = "[\\\\]|[/]"; }
    SpdrImport *q_ptr;

    bool importDirectory(const QString &inputDirectoryPath);
    bool importFile(const QString &filePath);
    QString getOutputFilePath(const QString &inputFilePath) const;
    QString getOperationStatusFromBool(bool status) const;

    bool checkFormat(const QString &format);
    int countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount);

    QString mPathSeparatorRegularExpression;
};

#endif // SPDRIMPORT_P_H

#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"

#include <QChar>
#include <QString>

class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
//    SpdrImportPrivate() {}
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    bool checkFormat(const QString &format);
    int countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount);

    QString mFormat;
};

#endif // SPDRIMPORT_P_H

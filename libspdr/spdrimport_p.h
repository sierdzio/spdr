#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"
#include "spdrbase_p.h"

#include <QString>

class SpdrImportPrivate : public SpdrBasePrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate() {}
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    bool checkFormat(const QString &format);

    QString mFormat;
};

#endif // SPDRIMPORT_P_H

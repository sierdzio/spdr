#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"
#include "spdrlog.h"

#include <QString>

class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate() {}
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    SpdrLog *mLog;

    Spdr::CopyMode mCopyMode;
    Spdr::UpdateMode mUpdateMode;
    QString mFormat;
    bool mIsLogFileSet;
};

#endif // SPDRIMPORT_P_H

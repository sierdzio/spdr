#ifndef SPDRBASE_P_H
#define SPDRBASE_P_H

#include "spdrbase.h"
#include "spdrlog.h"

#include <QString>

class SpdrBasePrivate
{
    Q_DECLARE_PUBLIC(SpdrBase)

public:
    SpdrBasePrivate(SpdrBase *q) : q_ptr(q) {}
    SpdrBase *q_ptr;

    QString getOperationStatusFromBool(bool status) const;
    bool areFilesTheSame(const QString &input, const QString &output) const;

    SpdrLog mLog;
    QString mInputPath;
    QString mOutputPath;
    bool mSimulate;
    Spdr::CopyMode mCopyMode;
    Spdr::UpdateMode mUpdateMode;
};

#endif // SPDRBASE_P_H

#ifndef SPDRSYNCHRONIZE_P_H
#define SPDRSYNCHRONIZE_P_H

#include "spdrsynchronize.h"
#include "spdrlog.h"

class SpdrSynchronizePrivate
{
    Q_DECLARE_PUBLIC(SpdrSynchronize)

public:
    SpdrSynchronizePrivate() {}
    SpdrSynchronizePrivate(SpdrSynchronize *q) : q_ptr(q) {}
    SpdrSynchronize *q_ptr;

    SpdrLog *mLog;

    Spdr::CopyMode mCopyMode;
    Spdr::UpdateMode mUpdateMode;
    SpdrSynchronize::SynchronizationOptions mOptions;
    uint mSplit;
    bool mIsLogFileSet;
};

#endif // SPDRSYNCHRONIZE_P_H

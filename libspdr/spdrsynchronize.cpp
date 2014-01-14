#include "spdrsynchronize_p.h"

SpdrSynchronize::SpdrSynchronize(QObject *parent) : QObject(parent), d_ptr(new SpdrSynchronizePrivate)
{
    Q_D(SpdrSynchronize);
}

int SpdrSynchronize::split() const
{
    Q_D(const SpdrSynchronize);
    return d->mSplit;
}

void SpdrSynchronize::setSplit(uint split)
{
    Q_D(SpdrSynchronize);
    d->mSplit = split;
}

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
}

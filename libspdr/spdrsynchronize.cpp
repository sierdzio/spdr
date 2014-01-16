#include "spdrsynchronize_p.h"

SpdrSynchronize::SpdrSynchronize(QObject *parent) : QObject(parent), d_ptr(new SpdrSynchronizePrivate)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}

Spdr::CopyMode SpdrSynchronize::copyMode() const
{
    Q_D(const SpdrSynchronize);
    return d->mCopyMode;
}

void SpdrSynchronize::setCopyMode(Spdr::CopyMode newCopyMode)
{
    Q_D(SpdrSynchronize);

    if (newCopyMode != d->mCopyMode) {
        d->mCopyMode = newCopyMode;
        emit copyModeChanged(newCopyMode);
    }
}

Spdr::UpdateMode SpdrSynchronize::updateMode() const
{
    Q_D(const SpdrSynchronize);
    return d->mUpdateMode;
}

void SpdrSynchronize::setUpdateMode(Spdr::UpdateMode newUpdateMode)
{
    Q_D(SpdrSynchronize);

    if (newUpdateMode != d->mUpdateMode) {
        d->mUpdateMode = newUpdateMode;
        emit updateModeChanged(newUpdateMode);
    }
}

SpdrSynchronize::SynchronizationOptions SpdrSynchronize::options() const
{
    Q_D(const SpdrSynchronize);
    return d->mOptions;
}

void SpdrSynchronize::setOptions(SynchronizationOptions options)
{
    Q_D(SpdrSynchronize);

    if (options != d->mOptions) {
        d->mOptions = options;
        emit optionsChanged(options);
    }
}

int SpdrSynchronize::split() const
{
    Q_D(const SpdrSynchronize);
    return d->mSplit;
}

void SpdrSynchronize::setSplit(uint split)
{
    Q_D(SpdrSynchronize);

    if (split != d->mSplit) {
        d->mSplit = split;
        emit splitChanged(split);
    }
}

bool SpdrSynchronize::synchronize()
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);

    return true;
}

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}

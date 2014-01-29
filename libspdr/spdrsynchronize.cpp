#include "spdrsynchronize_p.h"

SpdrSynchronize::SpdrSynchronize(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrSynchronizePrivate(this))
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
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

void SpdrSynchronize::registerMetatypes()
{
    Spdr::registerMetatypes();
    qRegisterMetaType<SpdrSynchronize::SynchronizationOptions>("SpdrSynchronize::SynchronizationOptions");
}

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent) : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}

#include "spdrsynchronize_p.h"

SpdrSynchronize::SpdrSynchronize(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrSynchronizePrivate(this))
{
    Q_D(SpdrSynchronize);

    d->mSplit = 0;
    d->mOptions = SpdrSynchronize::None;
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

        log(tr("Synchronization options changed to: %1").arg(QString::number((int) options, 16)), Spdr::Debug);

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

        log(tr("Split changed to: %1").arg(split), Spdr::Debug);

        emit splitChanged(split);
    }
}

bool SpdrSynchronize::synchronize() const
{
    Q_D(const SpdrSynchronize);
    Q_UNUSED(d);

    return true;
}

void SpdrSynchronize::registerMetatypes()
{
    Spdr::registerMetatypes();
    qRegisterMetaType<SpdrSynchronize::SynchronizationOptions>("SpdrSynchronize::SynchronizationOptions");
}

QString SpdrSynchronize::synchronizationOptionsToString(SynchronizationOptions optionSet)
{
    QString result;
    if (optionSet == None) {
        result += "None";
        return result;
    }

    if (optionSet & Bidirectional) {
        result += "Bidirectional | ";
    }

    if (optionSet & Cache) {
        result += "Cache | ";
    }

    if (optionSet & RemoveEmptyDirectories) {
        result += "RemoveEmptyDirectories | ";
    }

    if (optionSet & RemoveMissingFiles) {
        result += "RemoveMissingFiles | ";
    }

    if (optionSet & DeepSearch) {
        result += "DeepSearch | ";
    }

    result.chop(3);

    return result;
}

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent) : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}

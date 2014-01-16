#include "spdrimport_p.h"

SpdrImport::SpdrImport(QObject *parent) : QObject(parent), d_ptr(new SpdrImportPrivate)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

Spdr::CopyMode SpdrImport::copyMode() const
{
    Q_D(const SpdrImport);
    return d->mCopyMode;
}

void SpdrImport::setCopyMode(Spdr::CopyMode newCopyMode)
{
    Q_D(SpdrImport);

    if (newCopyMode != d->mCopyMode) {
        d->mCopyMode = newCopyMode;
        emit copyModeChanged(newCopyMode);
    }
}

Spdr::UpdateMode SpdrImport::updateMode() const
{
    Q_D(const SpdrImport);
    return d->mUpdateMode;
}

void SpdrImport::setUpdateMode(Spdr::UpdateMode newUpdateMode)
{
    Q_D(SpdrImport);

    if (newUpdateMode != d->mUpdateMode) {
        d->mUpdateMode = newUpdateMode;
        emit updateModeChanged(newUpdateMode);
    }
}

QString SpdrImport::format() const
{
    Q_D(const SpdrImport);
    return d->mFormat;
}

void SpdrImport::setFormat(const QString &format)
{
    Q_D(SpdrImport);

    if (format != d->mFormat) {
        d->mFormat = format;
        emit formatChanged(format);
    }
}

bool SpdrImport::import()
{
    Q_D(const SpdrImport);
    Q_UNUSED(d);

    return true;
}

bool SpdrImport::import(const QString &format)
{
    SpdrImport object;
    object.setFormat(format);
    return object.import();
}

SpdrImport::SpdrImport(SpdrImportPrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

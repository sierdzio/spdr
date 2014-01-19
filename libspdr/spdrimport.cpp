#include "spdrimport_p.h"

SpdrImport::SpdrImport(QObject *parent) : QObject(parent), d_ptr(new SpdrImportPrivate)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
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

#include "spdrimport_p.h"

SpdrImport::SpdrImport(QObject *parent) : QObject(parent), d_ptr(new SpdrImportPrivate)
{
    Q_D(SpdrImport);
}

QString SpdrImport::format() const
{
    Q_D(const SpdrImport);
    return d->mFormat;
}

void SpdrImport::setFormat(const QString &format)
{
    Q_D(SpdrImport);
    d->mFormat = format;
}

bool SpdrImport::import()
{
    Q_D(const SpdrImport);
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
}

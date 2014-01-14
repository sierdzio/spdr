#include "spdrimport_p.h"

SpdrImport::SpdrImport(QObject *parent) : QObject(parent), d_ptr(new SpdrImportPrivate)
{
    Q_D(SpdrImport);
}

bool SpdrImport::import()
{
    Q_D(const SpdrImport);
}

bool SpdrImport::import(const QString &format)
{
    Q_D(SpdrImport);

    SpdrImport object;
    object.format = format;

    return object.import();
}

SpdrImport::SpdrImport(SpdrImportPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d)
{
    Q_D(SpdrImport);
}

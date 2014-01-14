#include "spdrsynchronize_p.h"

SpdrSynchronize::SpdrSynchronize(QObject *parent) : QObject(parent), d_ptr(new SpdrSynchronizePrivate)
{
    Q_D(SpdrSynchronize);
}

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d)
{
    Q_D(SpdrSynchronize);
}

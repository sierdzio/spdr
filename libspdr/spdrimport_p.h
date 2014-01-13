#ifndef SPDRIMPORT_P_H
#define SPDRIMPORT_P_H

#include "spdrimport.h"

class SpdrImportPrivate
{
    Q_DECLARE_PUBLIC(SpdrImport)

public:
    SpdrImportPrivate() {}
    SpdrImportPrivate(SpdrImport *q) : q_ptr(q) {}
    SpdrImport *q_ptr;

    int mSplit;
};

#endif // SPDRIMPORT_P_H

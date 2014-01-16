#ifndef SPDRDLLSPEC_H
#define SPDRDLLSPEC_H

#include <qglobal.h>

#ifdef LIBSPDR
#define SPDR_DLLSPEC  Q_DECL_EXPORT
#else
#define SPDR_DLLSPEC Q_DECL_IMPORT
#endif

namespace Spdr {
    enum UpdateMode {
        Ask       = 0,
        Overwrite = 1,
        Ignore    = 2
    };

    enum CopyMode {
        Copy       = 0,
        Move       = 1,
        AlwaysMove = 2
    };
}

#endif // SPDRDLLSPEC_H

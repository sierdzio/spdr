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

enum LogLevel {
    NoLogging = 0,
    OnlyCritical = 1,
    OnlyErrors = 2,
    MildLogging = 3,
    MediumLogging = 4,
    LogEverything = 5
};
}

#endif // SPDRDLLSPEC_H

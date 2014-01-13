#ifndef SPDRDLLSPEC_H
#define SPDRDLLSPEC_H

#include <qglobal.h>

#ifdef LIBSPDR
#define SPDR_DLLSPEC  Q_DECL_EXPORT
#else
#define SPDR_DLLSPEC Q_DECL_IMPORT
#endif

#endif // SPDRDLLSPEC_H

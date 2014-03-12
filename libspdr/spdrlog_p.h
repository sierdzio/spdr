#ifndef SPDRLOG_P_H
#define SPDRLOG_P_H

#include "spdrlog.h"

/*!
  \ingroup libspdr
  \internal
  */
class SpdrLogPrivate
{
    Q_DECLARE_PUBLIC(SpdrLog)

public:
    SpdrLogPrivate(SpdrLog *q) : q_ptr(q) {}
    SpdrLog *q_ptr;

    bool mIsLogFileSet;
    Spdr::LogLevel mLogLevel;
    QString mLogFilePath;
};

#endif // SPDRLOG_P_H

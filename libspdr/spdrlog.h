#ifndef SPDRLOG_H
#define SPDRLOG_H

#include "spdrglobal.h"

#include <QString>

class SpdrLog
{
public:
    explicit SpdrLog();

    bool isUsingLogFile() const;

    Spdr::LogLevel logLevel() const;
    void setLogLevel(Spdr::LogLevel newLevel);

    QString logFilePath() const;
    void setLogFilePath(const QString &filePath);
    void log(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything);

private:
    bool mIsLogFileSet;
    Spdr::LogLevel mLogLevel;
    QString mLogFilePath;
};

#endif // SPDRLOG_H

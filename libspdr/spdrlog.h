#ifndef SPDRLOG_H
#define SPDRLOG_H

#include "spdrglobal.h"

#include <QString>

// TODO: make thread-friendly!
// TODO: implement log categories
class SpdrLog
{
public:
    explicit SpdrLog();

    bool isUsingLogFile() const;

    Spdr::LogLevel logLevel() const;
    void setLogLevel(Spdr::LogLevel newLevel);

    QString logFilePath() const;
    void setLogFilePath(const QString &filePath);
    void log(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything) const;

//signals:
//    void logMessage(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything) const;

private:
    bool mIsLogFileSet;
    Spdr::LogLevel mLogLevel;
    QString mLogFilePath;
};

#endif // SPDRLOG_H

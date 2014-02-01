#include "spdrlog.h"

#include <QFile>

SpdrLog::SpdrLog()
{
    mLogLevel = Spdr::Error;
    mIsLogFileSet = false;
}

bool SpdrLog::isUsingLogFile() const
{
    return mIsLogFileSet;
}

Spdr::LogLevel SpdrLog::logLevel() const
{
    return mLogLevel;
}

void SpdrLog::setLogLevel(Spdr::LogLevel newLevel)
{
    mLogLevel = newLevel;
}

QString SpdrLog::logFilePath() const
{
    return mLogFilePath;
}

void SpdrLog::setLogFilePath(const QString &filePath)
{
    mLogFilePath = filePath;
    mIsLogFileSet = !mLogFilePath.isEmpty();

    if (mIsLogFileSet) {
        QFile file(mLogFilePath);
        if (!file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
            QString logFile(mLogFilePath);
            setLogFilePath(QString::null);
            log(QString("Log file %1 could not be opened for writing! Reverting to stdout").arg(logFile));
        }

        file.close();
    }
}

void SpdrLog::log(const QString &message, Spdr::LogLevel logLevelToUse) const
{
    if (((int) logLevelToUse) > ((int) logLevel())) {
        return;
    }

    if (mIsLogFileSet) {
        QFile file(mLogFilePath);
        if (file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
            file.write(message.toUtf8());
        } else {
            //QString logFile(mLogFilePath);
            //setLogFilePath(QString::null);
            //log(QString("Log file %1 could not be opened for writing! Reverting to stdout").arg(logFile));
            qDebug("Log file could not be opened for writing!");
        }
    } else {
        qDebug(message.toLocal8Bit().constData(), NULL);
    }
}

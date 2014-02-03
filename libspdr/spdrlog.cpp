#include "spdrlog.h"

#include <QFile>

/*!
  \class SpdrLog

  Simple class used for log messages throughout Spdr.
  */

/*!
  Sets default values.
  */
SpdrLog::SpdrLog()
{
    mLogLevel = Spdr::Error;
    mIsLogFileSet = false;
}

/*!
  Returns true if a log file is being used. When this function returns false,
  it means that log messages will be printed on stdout.
  */
bool SpdrLog::isUsingLogFile() const
{
    return mIsLogFileSet;
}

/*!
  Returns the currently set log level.

  \sa Spdr::LogLevel
  */
Spdr::LogLevel SpdrLog::logLevel() const
{
    return mLogLevel;
}

/*!
  Sets the log level to \a newLevel.
  */
void SpdrLog::setLogLevel(Spdr::LogLevel newLevel)
{
    mLogLevel = newLevel;
}

/*!
  Returns the path to currently used log file. If the path is empty, log
  messages are printed on stdout.
  */
QString SpdrLog::logFilePath() const
{
    return mLogFilePath;
}

/*!
  Sets log file path to \a filePath. If the log file can not be opened for
  writing, it will reset the path to an empty string.

  \sa isUsingLogFile, logFilePath
  */
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

/*!
  Logs a \a message to either stdout or a given file. \a logLevelToUse should be
  used to notify how important the \a message is. SpdrLog will only print messages
  that have a lower or equal log level to currently set logLevel.

  \sa setLogLevel, logLevel, Spdr::LogLevel
  */
void SpdrLog::log(const QString &message, Spdr::LogLevel logLevelToUse) const
{
    if (logLevel() == Spdr::NoLogging) {
        return;
    }

    if (((int) logLevelToUse) > ((int) logLevel())) {
        return;
    }

    if (mIsLogFileSet) {
        QFile file(mLogFilePath);
        if (file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
            file.write(message.toUtf8().append("\n"));

            if(logLevelToUse == Spdr::Critical || logLevelToUse == Spdr::Error) {
                qDebug(message.toLocal8Bit().constData(), NULL);
            }
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

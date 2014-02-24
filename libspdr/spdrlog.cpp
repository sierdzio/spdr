#include "spdrlog_p.h"

#include <QFile>
#include <QDateTime>

/*!
  \class SpdrLog
  \ingroup libspdr

  Simple class used for log messages throughout Spdr.
  */

/*!
  Sets default values.
  */
SpdrLog::SpdrLog(QObject *parent) : QObject(parent), d_ptr(new SpdrLogPrivate(this))
{
    Q_D(SpdrLog);

    d->mLogLevel = Spdr::Error;
    d->mIsLogFileSet = false;
}

/*!
  Returns true if a log file is being used. When this function returns false,
  it means that log messages will be printed on stdout.
  */
bool SpdrLog::isUsingLogFile() const
{
    Q_D(const SpdrLog);
    return d->mIsLogFileSet;
}

/*!
  Returns the currently set log level.

  \sa Spdr::LogLevel
  */
Spdr::LogLevel SpdrLog::logLevel() const
{
    Q_D(const SpdrLog);
    return d->mLogLevel;
}

/*!
  Sets the log level to \a newLevel.
  */
void SpdrLog::setLogLevel(Spdr::LogLevel newLevel)
{
    Q_D(SpdrLog);
    d->mLogLevel = newLevel;
}

/*!
  Returns the path to currently used log file. If the path is empty, log
  messages are printed on stdout.
  */
QString SpdrLog::logFile() const
{
    Q_D(const SpdrLog);
    return d->mLogFilePath;
}

/*!
  Sets log file path to \a filePath. If the log file can not be opened for
  writing, it will reset the path to an empty string.

  \sa isUsingLogFile, logFilePath
  */
void SpdrLog::setLogFile(const QString &filePath)
{
    Q_D(SpdrLog);
    d->mLogFilePath = filePath;
    d->mIsLogFileSet = !d->mLogFilePath.isEmpty();

    if (d->mIsLogFileSet) {
        QFile file(d->mLogFilePath);
        if (!file.open(QFile::Text | QFile::WriteOnly)) {
            QString logFile(d->mLogFilePath);
            setLogFile(QString::null);
            log(QString("Log file %1 could not be opened for writing! Reverting to stdout").arg(logFile));
        }

        file.close();
    }
}

/*!
  Logs a \a message to either stdout or a given file. \a logLevelToUse should be
  used to notify how important the \a message is. SpdrLog will only print messages
  that have a lower or equal log level to currently set logLevel.

  WARNING! Do not connect SpdrLog::logMessage() signal to this slot!

  \sa setLogLevel, logLevel, Spdr::LogLevel
  */
void SpdrLog::log(const QString &message, Spdr::LogLevel logLevelToUse) const
{
    Q_D(const SpdrLog);

    if (logLevel() == Spdr::NoLogging) {
        return;
    }

    if (((int) logLevelToUse) > ((int) logLevel())) {
        return;
    }

    QString toPrint(QDateTime::currentDateTime().toString(Qt::ISODate)
                    + ": " + message);

    if (d->mIsLogFileSet) {
        QFile file(d->mLogFilePath);
        if (file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
            file.write(toPrint.toUtf8().append("\n"));

            if(logLevelToUse == Spdr::Critical || logLevelToUse == Spdr::Error) {
                qDebug(toPrint.toLocal8Bit().constData(), NULL);
                emit error(toPrint, (logLevelToUse == Spdr::Critical));
            } else {
                emit logMessage(toPrint, logLevelToUse);
            }

            file.close();
            return;
        } else {
            QString badPath(tr("Log file could not be opened for writing!"));
            qDebug(badPath.toLocal8Bit().constData(), NULL);
            emit error(badPath, false);
            return;
        }
    } else {
        qDebug(toPrint.toLocal8Bit().constData(), NULL);
        emit logMessage(toPrint, logLevelToUse);
        return;
    }
}

/*!
  PIMPL constructor. Please ignore.
 */
SpdrLog::SpdrLog(SpdrLogPrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrLog);
    Q_UNUSED(d);
}

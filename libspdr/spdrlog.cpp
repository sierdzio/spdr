#include "spdrlog.h"

#include <QFile>

SpdrLog::SpdrLog(QObject *parent) : QObject(parent)
{
}

bool SpdrLog::isUsingLogFile() const
{
    return mIsLogFileSet;
}

uint SpdrLog::logLevel() const
{
    return mLogLevel;
}

void SpdrLog::setLogLevel(uint newLevel)
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
}

void SpdrLog::log(const QString &message)
{
    if (mIsLogFileSet) {
        QFile file(mLogFilePath);
        if (!file.open(QFile::Text | QFile::WriteOnly | QFile::Append)) {
            QString logFile(mLogFilePath);
            setLogFilePath(QString::null);
            log(QString("Log file %1 could not be opened for writing! Reverting to stdout").arg(logFile));
        }

        file.write(message.toUtf8());
    } else {
        qDebug(message.toLocal8Bit().constData(), NULL);
    }
}

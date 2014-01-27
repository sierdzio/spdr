#ifndef SPDRLOG_H
#define SPDRLOG_H

#include <QString>
#include <QObject>

class SpdrLog : public QObject
{
    Q_OBJECT

public:
    explicit SpdrLog(QObject *parent = 0);

    bool isUsingLogFile() const;

    uint logLevel() const;
    void setLogLevel(uint newLevel);

    QString logFilePath() const;
    void setLogFilePath(const QString &filePath);

public slots:
    void log(const QString &message, uint logLevelToUse = 5);

private:
    bool mIsLogFileSet;
    uint mLogLevel;
    QString mLogFilePath;
};

#endif // SPDRLOG_H

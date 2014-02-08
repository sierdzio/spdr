#ifndef SPDRLOG_H
#define SPDRLOG_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

class SpdrLogPrivate;

class SPDR_DLLSPEC SpdrLog : public QObject
{
    Q_OBJECT

public:
    explicit SpdrLog(QObject *parent = 0);

    Q_INVOKABLE Spdr::LogLevel logLevel() const;
    Q_INVOKABLE void setLogLevel(Spdr::LogLevel newLevel);

    Q_INVOKABLE bool isUsingLogFile() const;
    Q_INVOKABLE QString logFile() const;
    Q_INVOKABLE void setLogFile(const QString &filePath);

public slots:
    void log(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything) const;

signals:
    void logMessage(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything) const;
    void error(const QString &message, bool isCritical = true) const;

protected:
    SpdrLog(SpdrLogPrivate &dd, QObject *parent = 0);
    SpdrLogPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrLog)
};

#endif // SPDRLOG_H

#ifndef SPDRBASE_H
#define SPDRBASE_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

class SpdrBasePrivate;

class SPDR_DLLSPEC SpdrBase : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(Spdr::CopyMode copyMode READ copyMode WRITE setCopyMode NOTIFY copyModeChanged)
    //Q_PROPERTY(Spdr::UpdateMode updateMode READ updateMode WRITE setUpdateMode NOTIFY updateModeChanged)

public:
    explicit SpdrBase(QObject *parent = 0);

    Q_INVOKABLE Spdr::CopyMode copyMode() const;
    Q_INVOKABLE void setCopyMode(Spdr::CopyMode newCopyMode);

    Q_INVOKABLE Spdr::UpdateMode updateMode() const;
    Q_INVOKABLE void setUpdateMode(Spdr::UpdateMode newUpdateMode);

    Q_INVOKABLE Spdr::LogLevel logLevel() const;
    Q_INVOKABLE void setLogLevel(Spdr::LogLevel newLevel);

    Q_INVOKABLE bool isUsingLogFile() const;
    Q_INVOKABLE void setLogFile(const QString &logFilePath);

signals:
    void copyModeChanged(Spdr::CopyMode newCopyMode);
    void updateModeChanged(Spdr::UpdateMode newUpdateMode);

protected:
    SpdrBase(SpdrBasePrivate &dd, QObject *parent = 0);
    SpdrBasePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrBase)
};

#endif // SPDRBASE_H

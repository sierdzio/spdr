#include "spdrbase_p.h"

SpdrBase::SpdrBase(QObject *parent) : QObject(parent), d_ptr(new SpdrBasePrivate)
{
    Q_D(SpdrBase);

    d->mCopyMode = Spdr::Copy;
    d->mUpdateMode = Spdr::Ask;
    d->mIsLogFileSet = false;
    d->mLog = new SpdrLog(this);
}

Spdr::CopyMode SpdrBase::copyMode() const
{
    Q_D(const SpdrBase);
    return d->mCopyMode;
}

void SpdrBase::setCopyMode(Spdr::CopyMode newCopyMode)
{
    Q_D(SpdrBase);

    if (newCopyMode != d->mCopyMode) {
        d->mCopyMode = newCopyMode;
        emit copyModeChanged(newCopyMode);
    }
}

Spdr::UpdateMode SpdrBase::updateMode() const
{
    Q_D(const SpdrBase);
    return d->mUpdateMode;
}

void SpdrBase::setUpdateMode(Spdr::UpdateMode newUpdateMode)
{
    Q_D(SpdrBase);

    if (newUpdateMode != d->mUpdateMode) {
        d->mUpdateMode = newUpdateMode;
        emit updateModeChanged(newUpdateMode);
    }
}

Spdr::LogLevel SpdrBase::logLevel() const
{
    Q_D(const SpdrBase);
    return (Spdr::LogLevel) d->mLog->logLevel();
}

void SpdrBase::setLogLevel(Spdr::LogLevel newLevel)
{
    Q_D(SpdrBase);
    d->mLog->setLogLevel((uint) newLevel);
}

bool SpdrBase::isUsingLogFile() const
{
    Q_D(const SpdrBase);
    return d->mLog->isUsingLogFile();
}

void SpdrBase::setLogFile(const QString &logFilePath)
{
    Q_D(SpdrBase);
    d->mLog->setLogFilePath(logFilePath);
}

QString SpdrBase::logFile() const
{
    Q_D(const SpdrBase);
    return d->mLog->logFilePath();
}

SpdrBase::SpdrBase(SpdrBasePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrBase);
    Q_UNUSED(d);
}
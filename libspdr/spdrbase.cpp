#include "spdrbase_p.h"

SpdrBase::SpdrBase(QObject *parent) : QObject(parent), d_ptr(new SpdrBasePrivate(this))
{
    Q_D(SpdrBase);

    d->mSimulate = false;
    d->mCopyMode = Spdr::Copy;
    d->mUpdateMode = Spdr::Ask;
}

QString SpdrBase::inputPath() const
{
    Q_D(const SpdrBase);
    return d->mInputPath;
}

void SpdrBase::setInputPath(const QString &newInputPath)
{
    Q_D(SpdrBase);

    // TODO: add path validation!

    if (newInputPath != d->mInputPath) {
        d->mInputPath = newInputPath;
        emit inputPathChanged(newInputPath);
    }
}

QString SpdrBase::outputPath() const
{
    Q_D(const SpdrBase);
    return d->mOutputPath;
}

bool SpdrBase::setOutputPath(const QString &newOutputPath)
{
    Q_D(SpdrBase);

    // TODO: add path validation!

    if (newOutputPath != d->mOutputPath) {
        d->mOutputPath = newOutputPath;

        if (d->mOutputPath.endsWith(QChar('/')) || d->mOutputPath.endsWith(QChar('\\'))) {
            d->mOutputPath.chop(1);
        }

        emit outputPathChanged(d->mOutputPath);
    }

    return true;
}

bool SpdrBase::simulate() const
{
    Q_D(const SpdrBase);
    return d->mSimulate;
}

void SpdrBase::setSimulate(bool simulationEnabled)
{
    Q_D(SpdrBase);

    if (simulationEnabled != d->mSimulate) {
        d->mSimulate = simulationEnabled;
        emit simulateChanged(simulationEnabled);
    }
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
    return d->mLog.logLevel();
}

void SpdrBase::setLogLevel(Spdr::LogLevel newLevel)
{
    Q_D(SpdrBase);
    d->mLog.setLogLevel(newLevel);
}

bool SpdrBase::isUsingLogFile() const
{
    Q_D(const SpdrBase);
    return d->mLog.isUsingLogFile();
}

void SpdrBase::setLogFile(const QString &logFilePath)
{
    Q_D(SpdrBase);
    d->mLog.setLogFilePath(logFilePath);
}

QString SpdrBase::logFile() const
{
    Q_D(const SpdrBase);
    return d->mLog.logFilePath();
}

void SpdrBase::log(const QString &message, Spdr::LogLevel logLevelToUse)
{
    Q_D(SpdrBase);
    d->mLog.log(message, logLevelToUse);
}

SpdrBase::SpdrBase(SpdrBasePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrBase);
    Q_UNUSED(d);
}

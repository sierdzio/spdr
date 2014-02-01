#include "spdrbase_p.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>

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
        log(tr("Input path changed to: %1").arg(newInputPath), Spdr::Debug);
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

        log(tr("Output path changed to: %1").arg(d->mOutputPath), Spdr::Debug);

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
        log(tr("Simulation is now: %1").arg(simulationEnabled? "on" : "off"), Spdr::Debug);
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

        log(tr("Copy mode changed to: %1").arg((int) newCopyMode), Spdr::Debug);

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

        log(tr("Update mode changed to: %1").arg((int) newUpdateMode), Spdr::Debug);

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
    log(tr("Log level changed to: %1").arg((int) newLevel), Spdr::Debug);
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
    log(tr("Now using log file: %1").arg(d->mLog.isUsingLogFile()? d->mLog.logFilePath() : "stdout"), Spdr::Debug);
}

QString SpdrBase::logFile() const
{
    Q_D(const SpdrBase);
    return d->mLog.logFilePath();
}

bool SpdrBase::performFileOperation(const QString &inputFile, const QString &outputFile) const
{
    Q_D(const SpdrBase);

    bool result = true;

    if (d->areFilesTheSame(inputFile, outputFile)) {
        log(tr("COPY: Skipping copying %1 to %2: files are identical")
            .arg(inputFile).arg(outputFile), Spdr::MediumLogging);
    } else {
        if (!simulate()) {
            bool skip = false;

            if (QFile(outputFile).exists()) {
                if (updateMode() == Spdr::Overwrite) {
                    result = QFile::remove(outputFile);
                } else if (updateMode() == Spdr::Ignore) {
                    skip = true;
                } else if (updateMode() == Spdr::Ask) { // TODO: implement Spdr::Ask
                    log(tr("This feature has not been implemented yet: Spdr::%1")
                        .arg(Spdr::updateModeToString(Spdr::Ask)), Spdr::Critical);
                    return false;
                }
            }

            if (!skip && result) {
                QFileInfo outputFileInfo(outputFile);
                QDir().mkpath(outputFileInfo.absolutePath());

                if (copyMode() == Spdr::Move) {
                    result = QFile::rename(inputFile, outputFile);
                } else {
                    result = QFile::copy(inputFile, outputFile);
                }
            }
        }

        log(tr("COPY: Copying %1 to %2 has: %3").arg(inputFile).arg(outputFile)
            .arg(d->getOperationStatusFromBool(result)), Spdr::MediumLogging);
    }

    return result;
}

void SpdrBase::log(const QString &message, Spdr::LogLevel logLevelToUse) const
{
    Q_D(const SpdrBase);
    d->mLog.log(message, logLevelToUse);
}

SpdrBase::SpdrBase(SpdrBasePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrBase);
    Q_UNUSED(d);
}

QString SpdrBasePrivate::getOperationStatusFromBool(bool status) const
{
    Q_Q(const SpdrBase);

    if (status) {
        return q->tr("succeeded");
    } else {
        return q->tr("failed");
    }
}

bool SpdrBasePrivate::areFilesTheSame(const QString &input, const QString &output) const
{
    QFileInfo inputInfo(input);
    QFileInfo outputInfo(output);

    if (inputInfo.fileName() == outputInfo.fileName()
            && inputInfo.size() == outputInfo.size()
            && inputInfo.created() == outputInfo.created())
    {
        return true;
    }

    return false;
}

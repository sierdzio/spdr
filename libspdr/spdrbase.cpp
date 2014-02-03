#include "spdrbase_p.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>

/*!
  \class SpdrBase

  Base class for concrete file operation methods implementations available in Spdr.

  It contains common properties and flags that other classes need: input and output
  paths, log functionality, copy settings, etc.
 */

/*!
  \property inputPath

  This is a required parameter (Spdr will bail out if this property is empty).
  Current directory can be specified with a single dot ".". The directory is
  always scanned recursively.

  It specifies the input path for Spdr to use. Input path is used as a base folder
  for import/ synchronization. This means that Spdr is treating it as a folder
  that is more up-to-date; a reference. Output path will be modified based on
  what is found in input path. Files in input path can be modified only if you
  set CopyMode to Move. In all other cases it will never be changed.

  \sa setCopyMode, setOutputPath
 */

/*!
  \property outputPath

  This is a required parameter (Spdr will bail out if this property is empty).
  Current directory can be specified with a single dot ".". Spdr will generate the
  whole directory tree below it.

  Output path specifies the destination folder for import or synchronize operations.

  For import operation, the output file structure will be determined by the format
  tags specified (see SpdrImport::setOutputPath for more details). Tags should be
  enclosed in "<" and ">" and contain QDateTime formatting of dates. Adding a star
  "*" outside of the tags will be treated as wildcard characters (allowing you
  to merge into an existing directory tree).

  For synchronize operation, the output file structure will be synchronized with
  input path: after the operation they should both look the same.
 */

/*!
  \property simulate

  If set to true, Spdr will run as usual, performing all the operations, but it will
  not do any acutal changes on the file system. This is best combined with a high
  LogLevel: it allows to perform a "dry run" and check if everythign would be
  done as needed.

  \sa setLogLevel, logLevel
 */

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

        log(tr("Copy mode changed to: %1").arg(Spdr::copyModeToString(newCopyMode)), Spdr::Debug);

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

        log(tr("Update mode changed to: %1").arg(Spdr::updateModeToString(newUpdateMode)), Spdr::Debug);

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
    log(tr("Log level changed to: %1").arg(Spdr::logLevelToString(newLevel)), Spdr::Debug);
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

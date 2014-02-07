#include "spdrbase_p.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>

/*!
  \class SpdrBase

  Base class for concrete file operation methods implementations available in Spdr.

  It contains common properties and flags that other classes need: input and output
  paths, log functionality, copy settings, etc. Some of the properties (input and
  output paths) need to be set in order for Spdr to work!
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

/*!
  Constructs the SpdrBase object, sets default values.
 */
SpdrBase::SpdrBase(QObject *parent) : QObject(parent), d_ptr(new SpdrBasePrivate(this))
{
    Q_D(SpdrBase);

    d->mSimulate = false;
    d->mUpdateMode = Spdr::Ask;
}

/*!
  Returns the current input path.
 */
QString SpdrBase::inputPath() const
{
    Q_D(const SpdrBase);
    return d->mInputPath;
}

/*!
  Sets the input path to \a newInputPath. Currend directory can be set with ".".
 */
void SpdrBase::setInputPath(const QString &newInputPath)
{
    Q_D(SpdrBase);

    if (newInputPath == d->mInputPath) {
        return;
    }

    QDir newDir(newInputPath);
    if (!newDir.exists()) {
        log(tr("Input directory path does not exist"), Spdr::Critical);
        return;
    }

    if (newDir.isRoot()) {
        log(tr("WARNING: Input path set to root directory: this can be dangerous"), Spdr::MildLogging);
    }

    d->mInputPath = newInputPath;
    log(tr("Input path changed to: %1").arg(newInputPath), Spdr::Debug);
    emit inputPathChanged(newInputPath);
}

/*!
  Returns the current output path.
 */
QString SpdrBase::outputPath() const
{
    Q_D(const SpdrBase);
    return d->mOutputPath;
}

/*!
  Sets the output path to \a newOutputPath. Returns false if the paths is not valid.

  This method works a bit differently for SpdrImport, see SpdrImport::setOutputPath.

  \sa SpdrImport::setOutputPath
 */
bool SpdrBase::setOutputPath(const QString &newOutputPath, bool checkIfExists)
{
    Q_D(SpdrBase);

    if (newOutputPath == d->mOutputPath) {
        return true;
    }

    QDir newDir(newOutputPath);
    if (checkIfExists && !newDir.exists()) {
        log(tr("Output directory path does not exist"), Spdr::Critical);
        return false;
    }

    if (newDir.isRoot()) {
        log(tr("WARNING: Output path set to root directory: this can be dangerous"), Spdr::MildLogging);
    }

    d->mOutputPath = newOutputPath;

    if (d->mOutputPath.endsWith(QChar('/')) || d->mOutputPath.endsWith(QChar('\\'))) {
        d->mOutputPath.chop(1);
    }

    log(tr("Output path changed to: %1").arg(d->mOutputPath), Spdr::Debug);

    emit outputPathChanged(d->mOutputPath);

    return true;
}

/*!
  Returns true if Spdr is working in simulation mode.
 */
bool SpdrBase::simulate() const
{
    Q_D(const SpdrBase);
    return d->mSimulate;
}

/*!
  When \a simulationEnabled is set to true, Spdr will run in simulation mode.
  This means that it will perform all operations as usual, but it will not actually
  change anything on the file system.
 */
void SpdrBase::setSimulate(bool simulationEnabled)
{
    Q_D(SpdrBase);

    if (simulationEnabled != d->mSimulate) {
        d->mSimulate = simulationEnabled;
        log(tr("Simulation is now: %1").arg(simulationEnabled? "on" : "off"), Spdr::Debug);
        emit simulateChanged(simulationEnabled);
    }
}

/*!
  Returns the current setting of Spdr::UpdateMode.
 */
Spdr::UpdateMode SpdrBase::updateMode() const
{
    Q_D(const SpdrBase);
    return d->mUpdateMode;
}

/*!
  Sets the Spdr::UpdateMode to \a newUpdateMode.
 */
void SpdrBase::setUpdateMode(Spdr::UpdateMode newUpdateMode)
{
    Q_D(SpdrBase);

    if (newUpdateMode != d->mUpdateMode) {
        d->mUpdateMode = newUpdateMode;

        log(tr("Update mode changed to: %1").arg(Spdr::updateModeToString(newUpdateMode)), Spdr::Debug);

        emit updateModeChanged(newUpdateMode);
    }
}

/*!
  Returns the current Spdr::LogLevel.
 */
Spdr::LogLevel SpdrBase::logLevel() const
{
    Q_D(const SpdrBase);
    return d->mLog.logLevel();
}

/*!
  Sets the Spdr::LogLevel to \a newLevel.
 */
void SpdrBase::setLogLevel(Spdr::LogLevel newLevel)
{
    Q_D(SpdrBase);
    log(tr("Log level changed to: %1").arg(Spdr::logLevelToString(newLevel)), Spdr::Debug);
    d->mLog.setLogLevel(newLevel);
}

/*!
  Returns true if Spdr is using the log file.
 */
bool SpdrBase::isUsingLogFile() const
{
    Q_D(const SpdrBase);
    return d->mLog.isUsingLogFile();
}

/*!
  Sets the log file path to \a logFilePath. If \a logFilePath is empty, Spdr will
  push all log messages to stdout.
 */
void SpdrBase::setLogFile(const QString &logFilePath)
{
    Q_D(SpdrBase);
    d->mLog.setLogFilePath(logFilePath);
    log(tr("Now using log file: %1").arg(d->mLog.isUsingLogFile()? d->mLog.logFilePath() : "stdout"), Spdr::Debug);
}

/*!
  Returns the path to the log file used by Spdr. An empty string means that stdout is
  being used to display the log messages.
 */
QString SpdrBase::logFile() const
{
    Q_D(const SpdrBase);
    return d->mLog.logFilePath();
}

/*!
  Prints the log \a message using \a logLevelToUse. Please note that if the log
  level is set to a higher value than \a logLevelToUse, this method will not
  print anything.

  \sa setLogLevel, setLogFile
 */
void SpdrBase::log(const QString &message, Spdr::LogLevel logLevelToUse) const
{
    Q_D(const SpdrBase);
    d->mLog.log(message, logLevelToUse);
}

/*!
  PIMPL constructor. Please ignore.
 */
SpdrBase::SpdrBase(SpdrBasePrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
    Q_D(SpdrBase);
    Q_UNUSED(d);
}

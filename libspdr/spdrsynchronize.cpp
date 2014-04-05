#include "spdrsynchronize_p.h"

#include <QCoreApplication>
#include <QByteArray>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>
#include <QDirIterator>

/*!
  \class SpdrSynchronize
  \ingroup libspdr

  \brief Can be used to intelligently synchronize 2 directories.

  The folder specified in input path is considered to be more "up-to-date" and
  more important. The files it contains will not be changed unless you run
  synchronization in Bidirectional mode.

  The output path is the target, or destination, directory. It will be brought
  up to date (into sync) with input directory. Spdr tries to optimise the
  process as much as possible: when a new file in input is only moved from a
  different place, Spdr will detect that and move it in output - which is much
  faster than deleting and copying the file again.
  */

/*!
  \enum SpdrSynchronize::SynchronizationOption

  Contains additional flags that can be passed to synchronization algorithm.

    \var SpdrSynchronize::SynchronizationOption SpdrSynchronize::None
         No additional options will be used
    \var SpdrSynchronize::SynchronizationOption SpdrSynchronize::RemoveEmptyDirectories
         If syncing makes a directory empty, it will be removed
    \var SpdrSynchronize::SynchronizationOption SpdrSynchronize::RemoveMissingFiles
         If some files are missing in input, they will also be removed from output
    \var SpdrSynchronize::SynchronizationOption SpdrSynchronize::DeepSearch
         Spdr will search for existing files using all available methods. Crucially, it will calculate SHA1 and MD5 checksums for all files: to avoid hash clashes. The DeepSearch will take more (a lot more) time, but in most situations the result will be exactly the same as running Spdr without it.
     */

/*!
  Standard constructor, initializes the object with default values.
  */
SpdrSynchronize::SpdrSynchronize(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrSynchronizePrivate(this))
{
    Q_D(SpdrSynchronize);

    d->mSplit = 0;
    d->mOptions = SpdrSynchronize::RemoveEmptyDirectories | RemoveMissingFiles;
}

/*!
  Returns the options that are currently set.
  */
SpdrSynchronize::SynchronizationOptions SpdrSynchronize::options() const
{
    Q_D(const SpdrSynchronize);
    return d->mOptions;
}

/*!
  Sets the current \a options.
  */
void SpdrSynchronize::setOptions(SynchronizationOptions options)
{
    Q_D(SpdrSynchronize);

    if (options != d->mOptions) {
        d->mOptions = options;

        log(tr("Synchronization options changed to: %1").arg(synchronizationOptionsToString(options)), Spdr::Debug);

        emit optionsChanged(options);
    }
}

/*!
  Performs the synchronization based on all settings. Input path is treated as
  the reference directory, and when synchronization is finished, output path
  should have exactly the same data inside, arranged in the same way.
  */
bool SpdrSynchronize::synchronize() const
{
    Q_D(const SpdrSynchronize);

    if (inputPath().isEmpty() || outputPath().isEmpty()) {
        emit finished(false);
        return false;
    }

    log(tr("START: beginning synchronization"), Spdr::MildLogging);
    log(tr("Using options: %1").arg(synchronizationOptionsToString(options())),
        Spdr::ExcessiveLogging);
    log(tr("Input path: %1").arg(inputPath()), Spdr::ExcessiveLogging);
    log(tr("Output path: %1").arg(outputPath()), Spdr::ExcessiveLogging);
    log(tr("Update mode: %1").arg(Spdr::updateModeToString(updateMode())), Spdr::ExcessiveLogging);
    log(tr("Simulation: %1").arg(simulate()), Spdr::ExcessiveLogging);

    log(tr("Building output folder structure database"), Spdr::MildLogging);
    QMultiHash<QByteArray, SpdrFileData> outputFileData;
    if (!d->readDirectoryFileData(outputPath(), &outputFileData)) {
        log(tr("Could not read file information from output directory"),
            Spdr::Error);
        emit finished(false);
        return false;
    }

    log(tr("Output file structure has been successfully analysed"),
        Spdr::MildLogging);
    log(tr("%1 files have been found and indexed").arg(outputFileData.count()),
        Spdr::MildLogging);

    if (!d->synchronizeDirectory(inputPath(), &outputFileData)) {
        log(tr("Could not finish directory synchronization due to errors"),
            Spdr::Error);
        emit finished(false);
        return false;
    }

    int missingFilesCount = outputFileData.count();
    log(tr("%1 files still left in the index after synchronization").arg(missingFilesCount),
        Spdr::MildLogging);

    if ((missingFilesCount > 0) && (options() & RemoveMissingFiles)) {
        log(tr("They will be removed because RemoveMissingFiles option is set"),
            Spdr::MildLogging);

        foreach (const SpdrFileData &data, outputFileData) {
            QString fileToRemove(outputPath() + "/" + data.path); // TODO: data.absoluteFilePath

            bool result = false;
            QString operation;

            if (simulate()) {
                result = true;
                operation = "SIMULATE REMOVE";
            } else {
                operation = "REMOVE";
                result = QFile::remove(fileToRemove);
            }

            log(tr("%1 (%2): File %3").arg(operation)
                .arg(Spdr::getOperationStatusFromBool(result))
                .arg(fileToRemove),
                result? Spdr::MediumLogging : Spdr::Error);

            if (!result) {
                emit finished(false);
                return false;
            }
        }

        outputFileData.clear();
    }

    if (options() & RemoveEmptyDirectories) {
        log(tr("Removing empty directories (if any)"), Spdr::MildLogging);
        if (!d->removeEmptyDirectories(outputPath())) {
            emit finished(false);
            return false;
        }
    }

    log(tr("DONE: Synchronization successful"), Spdr::MildLogging);

    emit finished(true);
    return true;
}

/*!
  Can be used to register the SynchronizationOptions enum with meta object system.

  This is usually only required when using QtTestLib.
  */
void SpdrSynchronize::registerMetatypes()
{
    Spdr::registerMetatypes();
    qRegisterMetaType<SpdrSynchronize::SynchronizationOptions>("SpdrSynchronize::SynchronizationOptions");
}

/*!
  Returns all flags set in \a optionSet as a string. Useful for logging and debugging.
  */
QString SpdrSynchronize::synchronizationOptionsToString(SynchronizationOptions optionSet)
{
    QString result;
    if (optionSet == None) {
        result += "None";
        return result;
    }

    if (optionSet & RemoveEmptyDirectories) {
        result += "RemoveEmptyDirectories | ";
    }

    if (optionSet & RemoveMissingFiles) {
        result += "RemoveMissingFiles | ";
    }

    if (optionSet & DeepSearch) {
        result += "DeepSearch | ";
    }

    result.chop(3);

    return result;
}

/*!
  \internal

  Private constructor. Best ignore it.
  */
SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent)
    : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}

/*!
  \internal

  Recursively reads file data in given \a directoryPath and all subdirectories.
  Returns true when successful. Resulting data can be read from \a fileHashTable.

  \sa readFileData
  */
bool SpdrSynchronizePrivate::readDirectoryFileData(const QString &directoryPath,
                                                   QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    QDirIterator it(directoryPath, QDir::NoDotAndDotDot | QDir::Files,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();

        if (!readFileData(it.fileInfo().absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    return true;
}

/*!
  \internal

  Reads data from a single file given in \a filePath. Returns true if successful.
  Data can be read later from \a fileHashTable.
  */
bool SpdrSynchronizePrivate::readFileData(const QString &filePath,
                                          QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    Q_Q(const SpdrSynchronize);
    SpdrFileData fileData(filePath, getRelativePathBase(QFileInfo(filePath).absoluteFilePath()),
                          q->options() & SpdrSynchronize::DeepSearch, q);

    if (fileData.isValid) {
        fileHashTable->insert(fileData.checksumMd5, fileData);
    } else {
        return false;
    }

    return true;
}

/*!
  \internal

  Performs synchronization on a single directory given in \a directoryPath (from
  input!), then recursively for all subdirectories. Returns true if everything
  went fine. Uses \a fileHashTable to read the information about output directory
  files. Returns true if successful.
  */
bool SpdrSynchronizePrivate::synchronizeDirectory(const QString &directoryPath,
                                                  QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    // Goes through all subfolders, returns only files: briliant!
    QDirIterator it(directoryPath, QDir::NoDotAndDotDot | QDir::Files,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();

        if (!synchronizeFile(it.fileInfo().absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    return true;
}

/*!
  \internal

  Decides how a single file should be synchronized. File given in \a filePath
  should come from the input directory structure. \a fileHashTable is used to
  scan information about output files. Returns true if successful.
  */
bool SpdrSynchronizePrivate::synchronizeFile(const QString &filePath,
                                             QMultiHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    Q_Q(const SpdrSynchronize);

    SpdrFileData inputFileData(filePath, getRelativePathBase(QFileInfo(filePath).absoluteFilePath()),
                               q->options() & SpdrSynchronize::DeepSearch, q);;
    QString outputBase(q->outputPath() + "/");

    if (!inputFileData.isValid) {
        return false;
    }

    // TODO: defer MD5 and SHA checking until after the fast track!
    // Fast track: if the paths match, and the hashes are the same, skip other checks
    {
        QString outputFileMirrorPath(outputBase + inputFileData.path);
        QFileInfo outputFileMirrorInfo(outputFileMirrorPath);

        if (outputFileMirrorInfo.exists()) {
            SpdrFileData outputFileData(outputFileMirrorPath, getRelativePathBase(QFileInfo(outputFileMirrorPath).absoluteFilePath()),
                                        q->options() & SpdrSynchronize::DeepSearch, q);

            if (inputFileData.isValid && inputFileData.isEqual(outputFileData)) {
                q->log(QCoreApplication::translate("SpdrSynchronizePrivate", "SKIP: Files %1 and %2 are identical")
                       .arg(filePath).arg(outputFileMirrorPath), Spdr::ExcessiveLogging);
                fileHashTable->remove(inputFileData.checksumMd5, outputFileData);
                return true;
            }
        }
    }

    // Fast track has not found the output file location. We need to traverse
    // the fileHashTable in order to find a candidate.

    // Let us first try to find by hash.
    // TODO: optimize! Don't try with contains() and then extract the value,
    // do it all in one go).
    if (fileHashTable->contains(inputFileData.checksumMd5))
    {
        QList<SpdrFileData> existingFiles = fileHashTable->values(inputFileData.checksumMd5);

        foreach (const SpdrFileData &outputData, existingFiles) {
            //SpdrFileData outputData = fileHashTable->take(inputFileData.checksumMd5);

            if (inputFileData.isMoved(outputData)) {
                fileHashTable->remove(inputFileData.checksumMd5, outputData);
                QString localCopyPath(outputBase + outputData.path);
                QString localDestinationPath(outputBase + inputFileData.path);
                QDir(q->outputPath()).mkpath(QFileInfo(localDestinationPath).absolutePath());

                bool result = false;
                QString operation;

                if (q->simulate()) {
                    result = true;
                    operation = "SIMULATE";
                } else {
                    if (q->options() & SpdrSynchronize::RemoveMissingFiles) {
                        result = QFile::rename(localCopyPath, localDestinationPath);
                        operation = "MOVE";
                    } else {
                        result = QFile::copy(localCopyPath, localDestinationPath);
                        operation = "COPY";
                    }
                }

                q->log(QCoreApplication::translate("SpdrSynchronizePrivate", "%1 (%2): Already existing file from %3 to %4").arg(operation)
                       .arg(Spdr::getOperationStatusFromBool(result))
                       .arg(localCopyPath).arg(localDestinationPath),
                       result? Spdr::MediumLogging : Spdr::Critical);

                if (!result) {
                    return false;
                }

                return true;
            }
        }
    }

    // Now, let us see if we can find a file with the same name and update it.
    // TODO: this needs to be done!
    // TODO: diff and EXIF comparison
    if (q->options() & SpdrSynchronize::DeepSearch) {
        //q->log(QCoreApplication::translate("No file match using standard search. Performing deep search"),
        //       Spdr::MildLogging);
    }

    // Now that we have eliminated other possibilites, we can conclude that
    // the file was added!
    //if (!q->performFileOperation(filePath, outputBase + inputFileData.path)) {
    {
        QString outputFilePath(outputBase + inputFileData.path);

        QFileInfo outputFileInfo(outputFilePath);
        QDir().mkpath(outputFileInfo.absolutePath());

        bool result = false;
        QString operation;

        if (q->simulate()) {
            result = true;
            operation = "SIMULATE COPY";
        } else {
            if (outputFileInfo.exists()) {
                // If the input file was modified, the old copy is still present
                // in the hashTable: so we need to remove it
                {
                    SpdrFileData oldFileData(outputFilePath, getRelativePathBase(QFileInfo(outputFilePath).absoluteFilePath()),
                                             q->options() & SpdrSynchronize::DeepSearch, q);
                    fileHashTable->remove(oldFileData.checksumMd5, oldFileData);
                }

                if (q->updateMode() == Spdr::Overwrite) {
                    QFile::remove(outputFilePath);
                    result = QFile::copy(filePath, outputFilePath);
                    operation = "OVERWRITE";
                } else if (q->updateMode() == Spdr::Ignore) {
                    result = true;
                    operation = "IGNORE";
                } else if (q->updateMode() == Spdr::Ask) {
                    result = false;
                    operation = "ASK";
                    q->log(QCoreApplication::translate("SpdrSynchronizePrivate", "Ask operation is not implemented yet, sorry!"), Spdr::Critical);
                    return false;
                }
            } else {
            result = QFile::copy(filePath, outputFilePath);
            operation = "COPY";
            }
        }

        q->log(QCoreApplication::translate("SpdrSynchronizePrivate", "%1 (%2): %3 to %4").arg(operation)
               .arg(Spdr::getOperationStatusFromBool(result))
               .arg(filePath).arg(outputFilePath),
               result? Spdr::MediumLogging : Spdr::Critical);

        if (!result) {
            return false;
        }
    }

    return true;
}

/*!
  \internal

  Recursively removes all emppty directories found in \a rootDirectoryPath. Returns true
  if successful.
  */
bool SpdrSynchronizePrivate::removeEmptyDirectories(const QString &rootDirectoryPath) const
{
    QDir inputDirectory(rootDirectoryPath);
    QFileInfoList dirList(inputDirectory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot));

    if (dirList.isEmpty()) {
        removeEmptyDirectory(rootDirectoryPath);
    } else {
        foreach (const QFileInfo &dir, dirList) {
            if (dir.isDir()) {
                if(!removeEmptyDirectories(dir.absoluteFilePath())) {
                    return false;
                }
            }
        }

        // After cleaning subdirs, we need to check this dir again
        dirList = inputDirectory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        if (dirList.isEmpty()) {
            return removeEmptyDirectory(rootDirectoryPath);
        }
    }

    return true;
}

/*!
  \internal

  Removes a single specified directory (\a directoryPath). Returns true if successful.
 */
bool SpdrSynchronizePrivate::removeEmptyDirectory(const QString &directoryPath) const
{
    Q_Q(const SpdrSynchronize);

    bool result = false;
    QString operation;

    if (q->simulate()) {
        result = true;
        operation = "SIMULATE REMOVE";
    } else {
        operation = "REMOVE";
        result = QDir(directoryPath + "/../").rmdir(directoryPath);
    }

    q->log(QCoreApplication::translate("SpdrSynchronizePrivate", "%1 (%2): Empty directory %3").arg(operation)
           .arg(Spdr::getOperationStatusFromBool(result))
           .arg(directoryPath),
           result? Spdr::MediumLogging : Spdr::Critical);

    return result;
}

/*!
  \internal

  Gets the relative base path for file specified by \a absoluteFilePath. Returns
  a path that is relative to either input or output directory (depending on where the
  given file is coming from).
  */
QString SpdrSynchronizePrivate::getRelativePathBase(const QString &absoluteFilePath) const
{
    Q_Q(const SpdrSynchronize);

    if (absoluteFilePath.contains(q->outputPath())) {
        return q->outputPath();
    } else {
        return q->inputPath();
    }
}

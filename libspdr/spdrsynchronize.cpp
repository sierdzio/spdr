#include "spdrsynchronize_p.h"

#include <QByteArray>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

SpdrSynchronize::SpdrSynchronize(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrSynchronizePrivate(this))
{
    Q_D(SpdrSynchronize);

    d->mSplit = 0;
    d->mOptions = SpdrSynchronize::RemoveEmptyDirectories | RemoveMissingFiles | Cache;
}

SpdrSynchronize::SynchronizationOptions SpdrSynchronize::options() const
{
    Q_D(const SpdrSynchronize);
    return d->mOptions;
}

void SpdrSynchronize::setOptions(SynchronizationOptions options)
{
    Q_D(SpdrSynchronize);

    if (options != d->mOptions) {
        d->mOptions = options;

        log(tr("Synchronization options changed to: %1").arg(synchronizationOptionsToString(options)), Spdr::Debug);

        emit optionsChanged(options);
    }
}

int SpdrSynchronize::split() const
{
    Q_D(const SpdrSynchronize);
    return d->mSplit;
}

void SpdrSynchronize::setSplit(uint split)
{
    Q_D(SpdrSynchronize);

    if (split != d->mSplit) {
        d->mSplit = split;

        log(tr("Split changed to: %1").arg(split), Spdr::Debug);

        emit splitChanged(split);
    }
}

bool SpdrSynchronize::synchronize() const
{
    Q_D(const SpdrSynchronize);

    log(tr("START: beginning synchronization"), Spdr::MildLogging);
    log(tr("Using options: %1").arg(synchronizationOptionsToString(options())), Spdr::ExcessiveLogging);
    log(tr("Input path: %1").arg(inputPath()), Spdr::ExcessiveLogging);
    log(tr("Output path: %1").arg(outputPath()), Spdr::ExcessiveLogging);
    log(tr("Simulation: %1").arg(simulate()), Spdr::ExcessiveLogging);

    QHash<QByteArray, SpdrFileData> outputFileData;
    if (!d->readDirectoryFileData(outputPath(), &outputFileData)) {
        log(tr("Could not read file information from output directory"), Spdr::Error);
        return false;
    }

    log(tr("Output file structure has been successfully analyzed"), Spdr::MildLogging);
    log(tr("%1 files have been found and indexed").arg(outputFileData.count()), Spdr::MildLogging);

    if (!d->synchronizeDirectory(inputPath(), &outputFileData)) {
        log(tr("Could not finish directory synchronization due to errors"), Spdr::Error);
        return false;
    }

    int missingFilesCount = outputFileData.count();
    log(tr("%1 files still left in the index after synchronization").arg(missingFilesCount),
        Spdr::MildLogging);

    if ((missingFilesCount > 0) && (options() & RemoveMissingFiles)) {
        log(tr("They will be removed because RemoveMissingFiles option is set"), Spdr::MildLogging);

        foreach (const SpdrFileData &data, outputFileData) {
            QString fileToRemove(outputPath() + "/" + data.path);
            if (QFile::remove(fileToRemove)) {
                log(tr("REMOVE: File %1 has been removed").arg(fileToRemove), Spdr::MediumLogging);
            } else {
                log(tr("REMOVE: Could not remove %1").arg(fileToRemove), Spdr::Error);
                return false;
            }
        }
    }

    if (options() & RemoveEmptyDirectories) {
        log(tr("Removing empty directories (if any)"), Spdr::MildLogging);
        if (!d->removeEmptyDirectory(outputPath())) {
            return false;
        }
    }

    log(tr("DONE: Synchronization successful"), Spdr::MildLogging);

    return true;
}

void SpdrSynchronize::registerMetatypes()
{
    Spdr::registerMetatypes();
    qRegisterMetaType<SpdrSynchronize::SynchronizationOptions>("SpdrSynchronize::SynchronizationOptions");
}

QString SpdrSynchronize::synchronizationOptionsToString(SynchronizationOptions optionSet)
{
    QString result;
    if (optionSet == None) {
        result += "None";
        return result;
    }

    if (optionSet & Bidirectional) {
        result += "Bidirectional | ";
    }

    if (optionSet & Cache) {
        result += "Cache | ";
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

SpdrSynchronize::SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent)
    : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrSynchronize);
    Q_UNUSED(d);
}


bool SpdrSynchronizePrivate::readDirectoryFileData(const QString &directoryPath,
                                                   QHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    QDir inputDirectory(directoryPath);

    QFileInfoList fileList(inputDirectory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
    QFileInfoList dirList(inputDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

    foreach (const QFileInfo &file, fileList) {
        if (!readFileData(file.absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    foreach (const QFileInfo &dir, dirList) {
        if (!readDirectoryFileData(dir.absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    return true;
}

bool SpdrSynchronizePrivate::readFileData(const QString &filePath,
                                          QHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    SpdrFileData fileData = getFileData(filePath);

    if (fileData.isValid) {
        fileHashTable->insert(fileData.checksumMd5, fileData);
    } else {
        return false;
    }

    return true;
}

bool SpdrSynchronizePrivate::synchronizeDirectory(const QString &directoryPath,
                                                  QHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    QDir inputDirectory(directoryPath);

    QFileInfoList fileList(inputDirectory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
    QFileInfoList dirList(inputDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

    foreach (const QFileInfo &file, fileList) {
        if (!synchronizeFile(file.absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    foreach (const QFileInfo &dir, dirList) {
        if (!synchronizeDirectory(dir.absoluteFilePath(), fileHashTable)) {
            return false;
        }
    }

    return true;
}

bool SpdrSynchronizePrivate::synchronizeFile(const QString &filePath,
                                             QHash<QByteArray, SpdrFileData> *fileHashTable) const
{
    Q_Q(const SpdrSynchronize);

    // TODO: implement all flags.
    // TODO: implement benchmarks.
    SpdrFileData inputFileData = getFileData(filePath);
    QString outputBase(q->outputPath() + "/");

    if (!inputFileData.isValid) {
        return false;
    }

    // Fast track: if the paths match, and the hashes are the same, skip other checks
    {
        QString outputFileMirrorPath(outputBase + inputFileData.path);
        QFileInfo outputFileMirrorInfo(outputFileMirrorPath);
        if (outputFileMirrorInfo.exists()) {
            SpdrFileData outputFileData = getFileData(outputFileMirrorPath);

            if (inputFileData.isValid && inputFileData.isEqual(outputFileData)) {
                if (q->performFileOperation(filePath, outputFileMirrorPath)) {
                    fileHashTable->remove(inputFileData.checksumMd5);
                    return true;
                }
            }
        }
    }

    // Fast track has not found the output file location. We need to traverse
    // the fileHashTable in order to find a candidate.

    // Let us first try to find by hash. Potentially suboptimal!
    // TODO: optimize! Don't try with contains() and then extract the value,
    // do it all in one go).
    if (fileHashTable->contains(inputFileData.checksumMd5))
    {
        //SpdrFileData outputData = fileHashTable.value(inputFileData.checksumMd5);
        SpdrFileData outputData = fileHashTable->take(inputFileData.checksumMd5);

        if (inputFileData.isMoved(outputData)) {
            QString localCopyPath(outputBase + outputData.path);
            QString localDestinationPath(outputBase + inputFileData.path);
            QDir(q->outputPath()).mkpath(QFileInfo(localDestinationPath).absolutePath());

            if (q->simulate()) {
                q->log(q->tr("SIMULATE: Already existing file moved from %1 to %2")
                       .arg(localCopyPath).arg(localDestinationPath), Spdr::MediumLogging);
                return true;
            }

            if (q->options() & SpdrSynchronize::RemoveMissingFiles) {
                if (QFile::rename(localCopyPath, localDestinationPath)) {
                    q->log(q->tr("MOVE: Already existing file moved from %1 to %2")
                           .arg(localCopyPath).arg(localDestinationPath), Spdr::MediumLogging);
                } else {
                    q->log(q->tr("MOVE: could not move the file from %1 to %2")
                           .arg(localCopyPath).arg(localDestinationPath), Spdr::Critical);
                    return false;
                }
            } else {
                if (QFile::copy(localCopyPath, localDestinationPath)) {
                    q->log(q->tr("COPY: Already existing file copied from %1 to %2")
                           .arg(localCopyPath).arg(localDestinationPath), Spdr::MediumLogging);
                } else {
                    q->log(q->tr("COPY: could not copy the file from %1 to %2")
                           .arg(localCopyPath).arg(localDestinationPath), Spdr::Critical);
                    return false;
                }
            }

            if (q->copyMode() == Spdr::Move) {
                if (!QFile::remove(filePath)) {
                    q->log(q->tr("Could not remove the input file: %1").arg(filePath),
                           Spdr::MediumLogging);
                }
            }

            return true;
        }
    }

    // Now, let us see if we can find a file with the same name and update it.
    // TODO: this needs to be done!
    // TODO: diff and EXIF comparison
    {}

    // Now that we have eliminated other possibilites, we can conclude that
    // the file was added!
    if (!q->performFileOperation(filePath, outputBase + inputFileData.path)) {
        return false;
    }

    return true;
}

bool SpdrSynchronizePrivate::removeEmptyDirectory(const QString &directoryPath) const
{
    Q_Q(const SpdrSynchronize);

    QDir inputDirectory(directoryPath);
    QFileInfoList dirList(inputDirectory.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot));

    if (dirList.isEmpty()) {
        if (QDir(directoryPath).rmdir(directoryPath)) {
            q->log(q->tr("REMOVE: Removed empty directory %1").arg(directoryPath),
                   Spdr::MediumLogging);
            return true;
        } else {
            q->log(q->tr("REMOVE: Could not remove empty directory %1").arg(directoryPath),
                   Spdr::Error);
            return false;
        }
    } else {
        foreach (const QFileInfo &dir, dirList) {
            if (dir.isDir()) {
                removeEmptyDirectory(dir.absoluteFilePath());
            }
        }
    }

    return true;
}

SpdrFileData SpdrSynchronizePrivate::getFileData(const QString &filePath) const
{
    Q_Q(const SpdrSynchronize);

    QFileInfo fileInfo(filePath);
    SpdrFileData fileData;
    fileData.isValid = true;
    fileData.name = fileInfo.fileName();
    fileData.path = getRelativeFilePath(fileInfo.absoluteFilePath());
    fileData.creationDate = fileInfo.created();
    fileData.size = fileInfo.size();

    QString fileReadError(q->tr("File could not be opened for reading while attempting to create a hash! %1")
                          .arg(fileData.path));

    QString fileHashingError(q->tr("Could not create an %1 hash for file %2"));

    QFile fileMd5(filePath);
    if (fileMd5.open(QFile::ReadOnly)) {
        QCryptographicHash md5(QCryptographicHash::Md5);

        if (md5.addData(&fileMd5)) {
            fileData.checksumMd5 = md5.result();
        } else {
            q->log(fileHashingError.arg("MD5").arg(fileData.path), Spdr::Critical);
            fileData.isValid = false;
            return fileData;
        }
    } else {
        q->log(fileReadError, Spdr::Critical);
        fileData.isValid = false;
        return fileData;
    }

    fileMd5.close();

    QFile fileSha(filePath);
    if (fileSha.open(QFile::ReadOnly)) {
        QCryptographicHash sha(QCryptographicHash::Sha1);

        if (sha.addData(&fileSha)) {
            fileData.checksumMd5 = sha.result();
        } else {
            q->log(fileHashingError.arg("SHA").arg(fileData.path), Spdr::Error);
        }
    } else {
        q->log(fileReadError, Spdr::Critical);
    }

    fileSha.close();

    return fileData;
}

QString SpdrSynchronizePrivate::getRelativeFilePath(const QString &absoluteFilePath) const
{
    Q_Q(const SpdrSynchronize);

    QDir absoluteDir;

    if (absoluteFilePath.contains(q->outputPath())) {
        absoluteDir = QDir(q->outputPath());
    } else {
        absoluteDir = QDir(q->inputPath());
    }
    return absoluteDir.relativeFilePath(absoluteFilePath);
}

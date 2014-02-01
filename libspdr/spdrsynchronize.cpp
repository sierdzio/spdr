#include "spdrsynchronize_p.h"

#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

SpdrSynchronize::SpdrSynchronize(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrSynchronizePrivate(this))
{
    Q_D(SpdrSynchronize);

    d->mSplit = 0;
    d->mOptions = SpdrSynchronize::None;
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

        log(tr("Synchronization options changed to: %1").arg(QString::number((int) options, 16)), Spdr::Debug);

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

    QHash<QByteArray, SpdrFileData> outputFileData;
    if (!d->readDirectoryFileData(outputPath(), &outputFileData)) {
        log(tr("Could not read file information from output directory"), Spdr::Error);
        return false;
    }

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
    Q_Q(const SpdrSynchronize);

    QFileInfo fileInfo(filePath);
    SpdrFileData fileData;
    fileData.name = fileInfo.fileName();
    fileData.path = fileInfo.absoluteFilePath();
    fileData.creationDate = fileInfo.created();
    fileData.size = fileInfo.size();

    QString fileReadError(q->tr("File could not be opened for reading while attempting to create a hash! %1")
                          .arg(fileData.path));

    QString fileHashingError(q->tr("Could not create an %1 hash for file %2"));

    QFile fileMd5(fileData.path);
    if (fileMd5.open(QFile::ReadOnly)) {
        QCryptographicHash md5(QCryptographicHash::Md5);

        if (md5.addData(&fileMd5)) {
            fileData.checksumMd5 = md5.result();
        } else {
            q->log(fileHashingError.arg("MD5").arg(fileData.path), Spdr::Critical);
            return false;
        }
    } else {
        q->log(fileReadError, Spdr::Critical);
        return false;
    }

    fileMd5.close();

    QFile fileSha(fileData.path);
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

    fileHashTable->insert(fileData.checksumMd5, fileData);

    return true;
}

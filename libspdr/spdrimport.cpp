#include "spdrimport_p.h"
#include "spdrfiledata.h"

#include <QChar>
#include <QString>
#include <QStringList>
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

/*!
  \class SpdrImport
  \ingroup libspdr

  \brief Can be used to intelligently import a directory, dividing all files
  into neatly organised set of directories.

  For example, you may have a directory full of unsorted photos. Then, by importing
  that directory with this filter set: ../myPhotos/<yyyy>/<MM>/<yyyy-MM-dd>, it will
  copy all files to "myPhotos", and also make sure they are all put into separate
  directories based on the date they were taken.
 */

/*!
  \enum SpdrImport::CopyMode

  Determines what operation should Spdr perform on files it is operating on

    \var SpdrImport::CopyMode SpdrImport::Copy
         Files will be copied (no changes in input paths)
    \var SpdrImport::CopyMode SpdrImport::Move
         Files will be moved (file may be deleted from input path, if the move operation is succesful)
  */

/*!
  Standard constructor: creates the object and sets default values for properties.
 */
SpdrImport::SpdrImport(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrImportPrivate(this))
{
    Q_D(SpdrImport);

    d->mPathSeparatorRegularExpression.setPattern("[\\\\]|[/]");
    d->mCopyMode = Copy;
}

/*!
  Can be used to set the format for the destination directory, along with the path.

  Example format: ../myPhotos/<yyyy>/<MM>/<yyyy-MM-dd>*

  A star is being read as "anything goes" and can be used to import into some
  preexisting, named folders like "2014-01-27 Pics from today" with the following
  format "<yyyy-MM-dd>*".

  Accepted date formatting strings are the same as for QDateTime class and have
  to be put between "<" and ">" (otherwise they will be treated as part of the
  path). Spdr understands tags for date as well as for time.
 */
bool SpdrImport::setOutputPath(const QString &newOutputPath, bool checkIfExists)
{
    Q_UNUSED(checkIfExists);
    Q_D(SpdrImport);

    if (newOutputPath != outputPath()) {
        if (d->checkFormat(newOutputPath)) {
            SpdrBase::setOutputPath(newOutputPath, false);
            return true;
        } else if (!outputPath().isEmpty()) {
            SpdrBase::setOutputPath(QString::null);
            return false;
        }
    }

    return false;
}

/*!
  Returns the current setting of CopyMode.
 */
SpdrImport::CopyMode SpdrImport::copyMode() const
{
    Q_D(const SpdrImport);
    return d->mCopyMode;
}

/*!
  Sets the CopyMode to \a newCopyMode.
 */
void SpdrImport::setCopyMode(SpdrImport::CopyMode newCopyMode)
{
    Q_D(SpdrImport);

    if (newCopyMode != d->mCopyMode) {
        d->mCopyMode = newCopyMode;

        log(tr("Copy mode changed to: %1").arg(copyModeToString(newCopyMode)), Spdr::Debug);

        emit copyModeChanged(newCopyMode);
    }
}

/*!
  Performs the import operation. That is to say, it takes all files from input
  path and puts them into output path according to formatting tags that have
  been used.

  For example, if you want to copy your newly taken photographs onto your local
  foto archive/ gallery, you can use this output path:

  /some/path/<yyyy>/<MM>/<yyyy-MM-dd>*

  This will copy the files into a well-organized folder structure. Later, when
  you take more pictures, you can run import() again, and the archive will be
  updated.

  \sa setOutputPath
 */
bool SpdrImport::import() const
{
    Q_D(const SpdrImport);

    bool result = true;

    if (inputPath().isEmpty() || outputPath().isEmpty()) {
        emit finished(false);
        return false;
    }

    result = d->importDirectory(inputPath());

    if (result) {
        log(tr("DONE: Directory import successful"), Spdr::MildLogging);
    }

    emit finished(result);
    return result;
}

/*!
  Returns a string representation of given \a mode. Useful in debugging/ logging.
 */
QString SpdrImport::copyModeToString(SpdrImport::CopyMode mode)
{
    QString result;
    if (mode == Copy) {
        result = "Copy";
    } else if (mode == Move) {
        result = "Move";
    }

    return result;
}

/*!
  This method can be used to register Spdr enums with the Meta Object system.

  Useful when one needs to use those enums in Qt tests.
 */
void SpdrImport::registerMetatypes()
{
    qRegisterMetaType<SpdrImport::CopyMode>("SpdrImport::CopyMode");
}

/*!
  \internal
  */
SpdrImport::SpdrImport(SpdrImportPrivate &dd, QObject *parent) : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

/*!
  \internal

  Performs the requested file operation based on CopyMode and UpdateMode settings.

  Returns true if successful.
 */
bool SpdrImportPrivate::performFileOperation(const QString &inputFile, const QString &outputFile) const
{
    Q_Q(const SpdrImport);

    bool result = true;

    if (areFilesTheSame(inputFile, outputFile)) {
        q->log(QCoreApplication::translate("SpdrImportPrivate",
                                           "COPY: Skipping copying %1 to %2: files are identical")
               .arg(inputFile).arg(outputFile), Spdr::MediumLogging);
    } else {
        if (!q->simulate()) {
            bool skip = false;

            if (QFile(outputFile).exists()) {
                if (q->updateMode() == Spdr::Overwrite) {
                    result = QFile::remove(outputFile);
                } else if (q->updateMode() == Spdr::Ignore) {
                    skip = true;
                } else if (q->updateMode() == Spdr::Ask) { // TODO: implement Spdr::Ask
                    q->log(QCoreApplication::translate("SpdrImportPrivate",
                                                       "This feature has not been implemented yet: Spdr::%1")
                           .arg(Spdr::updateModeToString(Spdr::Ask)), Spdr::Critical);
                    return false;
                }
            }

            if (!skip && result) {
                QFileInfo outputFileInfo(outputFile);
                QDir().mkpath(outputFileInfo.absolutePath());

                if (q->copyMode() == SpdrImport::Move) {
                    result = QFile::rename(inputFile, outputFile);
                } else {
                    result = QFile::copy(inputFile, outputFile);
                }
            }
        }

        q->log(QCoreApplication::translate("SpdrImportPrivate", "COPY: Copying %1 to %2 was a: %3").arg(inputFile).arg(outputFile)
               .arg(Spdr::getOperationStatusFromBool(result)), Spdr::MediumLogging);
    }

    return result;
}

/*!
  \internal

  Returns true if file names for \a input and \a output are the same.
 */
bool SpdrImportPrivate::areFilesTheSame(const QString &input, const QString &output) const
{
    Q_Q(const SpdrImport);

    QFileInfo outputInfo(output);
    if (!outputInfo.exists()) {
        return false;
    }

    SpdrFileData inputData(input, q->inputPath(), false, q);
    SpdrFileData outputData(output, q->inputPath(), false, q);

    if (inputData.isEqual(outputData)) {
        return true;
    }

    return false;
}

/*!
  \internal

  Recursively imports a directory specified by \a directoryPath. Internally,
  this method also calls importFile() for every file it encounters.

  \sa importFile
  */
bool SpdrImportPrivate::importDirectory(const QString &directoryPath) const
{
    QDir inputDirectory(directoryPath);

    QFileInfoList fileList(inputDirectory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot));
    QFileInfoList dirList(inputDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

    foreach (const QFileInfo &file, fileList) {
        if (!importFile(file.absoluteFilePath())) {
            return false;
        }
    }

    foreach (const QFileInfo &dir, dirList) {
        if (!importDirectory(dir.absoluteFilePath())) {
            return false;
        }
    }

    return true;
}

/*!
  \internal

  Imports the file specified by \a filePath. File destination is acquired
  automatically based on output path.
  */
bool SpdrImportPrivate::importFile(const QString &filePath) const
{
    QString outputPath(getOutputFilePath(filePath));
    outputPath = substituteStarsInPath(outputPath);
    return performFileOperation(filePath, outputPath);
}

/*!
  \internal

  Constructs the output file path based on formatting tags specified in output
  path property.
  */
QString SpdrImportPrivate::getOutputFilePath(const QString &inputFilePath) const
{
    Q_Q(const SpdrImport);

    // TODO: allow to change the input file name, too!
    QFileInfo fileInfo(inputFilePath);
    QDateTime creationDate(fileInfo.lastModified());

    // Reads both Unix and Windows paths
    QStringList pathSegments(q->outputPath().split(mPathSeparatorRegularExpression,
                                                   QString::KeepEmptyParts));
    pathSegments.append(fileInfo.fileName());
    QStringList outputPathSegments;

    foreach (const QString &segment, pathSegments) {
        if (!segment.contains("<") && !segment.contains(">")) {
            outputPathSegments.append(segment);
            continue;
        }

        int lessThanIndex = segment.indexOf("<") + 1;
        int greaterThanIndex = segment.indexOf(">", lessThanIndex);
        QString dateFormat(segment.mid(lessThanIndex, greaterThanIndex - lessThanIndex));
        QString resultSegment(segment);
        resultSegment.replace(QLatin1String("<") + dateFormat + QLatin1String(">"), creationDate.toString(dateFormat));

        outputPathSegments.append(resultSegment);
    }

    QString result(outputPathSegments.join("/"));

    q->log(QCoreApplication::translate("SpdrImportPrivate", "Output file path set to: %1").arg(result), Spdr::Debug);

    return result;
}

/*!
  \internal

  Use this to replace all occurences of '*' with directory candidates (if applicable).

  This method will not work correctly when date tags are still present in path.
  You should run getOutputFilePath() first.

  \sa getOutputFilePath
 */
QString SpdrImportPrivate::substituteStarsInPath(const QString &outputFilePath) const
{
    Q_Q(const SpdrImport);

    QChar star('*');
    if (!outputFilePath.contains(star)) {
        return outputFilePath;
    }

    QStringList outputPathSegments(outputFilePath.split(mPathSeparatorRegularExpression,
                                                        QString::KeepEmptyParts));

    // Go through every segment, see if dir exists, see if there is a name match,
    // fix the segment.
    QString pathBuilder;
    foreach(const QString &segment, outputPathSegments) {
        if (!pathBuilder.isEmpty()) {
            pathBuilder.append("/");
        }
//        else if (pathBuilder.isEmpty() && q->outputPath().startsWith('/')) {
//            pathBuilder.append("/");
//        }

        // Get existing directory names, look for a candidate
        if (segment.contains(star)) {
            QDir currentDirectory(pathBuilder);
            QFileInfoList dirs(currentDirectory.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

            QString regExpString(segment); // "*" + dirName + "*"
            QRegularExpression regExp(regExpString);
            q->log(QCoreApplication::translate("SpdrImportPrivate", "Star matching regular expression set to: %1").arg(regExpString), Spdr::Debug);

            bool found = false;
            foreach (const QFileInfo &dirInfo, dirs) {
                QString dirName(dirInfo.fileName());

                if (regExp.match(dirName).hasMatch()) {
                    pathBuilder.append(dirName);
                    found = true;
                    q->log(QCoreApplication::translate("SpdrImportPrivate", "An existing directory that matches wildcard has been found: %1")
                           .arg(dirName), Spdr::ExcessiveLogging);
                    break;
                }
            }

            if (!found) {
                QString plainDir(segment);
                pathBuilder.append(plainDir.remove('*'));
            }
        } else {
            pathBuilder.append(segment);
        }
    }

    if (q->outputPath().startsWith('/') && !pathBuilder.startsWith('/')) {
        pathBuilder.prepend('/');
    }

    q->log(QCoreApplication::translate("SpdrImportPrivate", "Path builder result: %1").arg(pathBuilder), Spdr::Debug);

    return pathBuilder;
}

/*!
  \internal

  Returns true if both the format and the file path are valid.
 */
bool SpdrImportPrivate::checkFormat(const QString &format) const
{
    Q_Q(const SpdrImport);

    bool result = true;

    // Reads both Unix and Windows paths
    QStringList pathSegments(format.split(mPathSeparatorRegularExpression,
                                          QString::SkipEmptyParts));

    foreach (const QString &segment, pathSegments) {
        int lessThanIndex = segment.indexOf("<");
        int greaterThanIndex = segment.indexOf(">");

        if (lessThanIndex == -1 && greaterThanIndex == -1) {
            // No template here
            continue;
        } else if (lessThanIndex == -1 || greaterThanIndex == -1) {
            result = false;
            q->log(QCoreApplication::translate("SpdrImportPrivate", "Missing tag enclosure: < or >: %1").arg(segment), Spdr::Critical);
            break;
        } else if (lessThanIndex > -1 && (lessThanIndex >= greaterThanIndex)) {
            result = false;
            q->log(QCoreApplication::translate("SpdrImportPrivate", "Date format tag is closed before it is opened: %1").arg(segment), Spdr::Critical);
            break;
        } else if (countOccurences(segment, '<') != countOccurences(segment, '>')) {
            result = false;
            q->log(QCoreApplication::translate("SpdrImportPrivate", "Too many date formatting tags: %1").arg(segment), Spdr::Critical);
            break;
        }
    }

    return result;
}

/*!
  \internal

  Returns the number of times \a characterToCount can be found in
  \a stringToSearchThrough.
  */
int SpdrImportPrivate::countOccurences(const QString &stringToSearchThrough,
                                       const QChar &characterToCount) const
{
    int result = 0;
    for (int i = 0; i < stringToSearchThrough.length(); i++) {
        if (stringToSearchThrough.at(i) == characterToCount) {
            result++;
        }
    }

    return result;
}

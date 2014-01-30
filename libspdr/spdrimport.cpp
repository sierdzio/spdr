#include "spdrimport_p.h"

#include <QStringList>
#include <QRegularExpression>
#include <QDateTime>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

SpdrImport::SpdrImport(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrImportPrivate(this))
{
    Q_D(SpdrImport);

    d->mPathSeparatorRegularExpression = "[\\\\]|[/]";
}

/*!
  Can be used to set the format for the import dir, along with the path.

  Example format: ../myPhotos/<yyyy>/<MM>/<yyyy-MM-dd>*

  A star is being read as "anything goes" and can be used to import into some
  preexisting, named folders like "2014-01-27 Pics from today" with the following
  format "<yyyy-MM-dd>*".

  Accepted date formatting strings are the same as for QDateTime class and have
  to be put between "<" and ">" (otherwise they will be treated as part of the
  path).
 */
bool SpdrImport::setOutputPath(const QString &newOutputPath)
{
    Q_D(SpdrImport);

    if (newOutputPath != outputPath()) {
        if (d->checkFormat(newOutputPath)) {
            SpdrBase::setOutputPath(newOutputPath);
            return true;
        } else if (!outputPath().isEmpty()) {
            SpdrBase::setOutputPath(QString::null);
            return false;
        }
    }

    return false;
}

bool SpdrImport::import() const
{
    Q_D(const SpdrImport);

    bool result = true;

    if (inputPath().isEmpty() || outputPath().isEmpty()) {
        return false;
    }

    result = d->importDirectory(inputPath());

    return result;
}

bool SpdrImport::import(const QString &inputPath, const QString &outputPath)
{
    SpdrImport object;

    object.setInputPath(inputPath);

    if (!object.setOutputPath(outputPath)) {
        return false;
    }

    return object.import();
}

SpdrImport::SpdrImport(SpdrImportPrivate &dd, QObject *parent) : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

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

bool SpdrImportPrivate::importFile(const QString &filePath) const
{
    Q_Q(const SpdrImport);

    QString outputPath(getOutputFilePath(filePath));

    bool result = true;

    if (!q->simulate()) {
        bool skip = false;
        if (QFile(outputPath).exists()) {
            if (q->updateMode() == Spdr::Overwrite) {
                result = QFile::remove(outputPath);
            } else if (q->updateMode() == Spdr::Ignore) {
                skip = true;
            } else if (q->updateMode() == Spdr::Ask) { // TODO: implement Spdr::Ask
                q->log(q->tr("This feature has not been implemented yet: Spdr::Ask"));
            }
        }

        if (!skip && result) {
            QFileInfo outputFile(outputPath);
            QDir().mkdir(outputFile.absolutePath());
            result = QFile::copy(filePath, outputPath);

            if (q->copyMode() == Spdr::Move) {
                bool movingSuccessful = QFile::remove(filePath);
                if (!movingSuccessful) {
                    q->log(q->tr("MOVE: Could not remove the input file: ").arg(filePath));
                }
            }
        }
    }

    q->log(q->tr("COPY: Copying %1 to %2 has: %3").arg(filePath).arg(outputPath)
           .arg(getOperationStatusFromBool(result)), Spdr::MediumLogging);
    return result;
}

QString SpdrImportPrivate::getOutputFilePath(const QString &inputFilePath) const
{
    Q_Q(const SpdrImport);

    // TODO: allow to change the input file name, too!
    QFileInfo fileInfo(inputFilePath);
    QDateTime creationDate(fileInfo.created());

    // Reads both Unix and Windows paths
    QStringList pathSegments(q->outputPath().split(QRegularExpression(mPathSeparatorRegularExpression),
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

        // TODO: add handling for '*' character

        outputPathSegments.append(resultSegment);
    }

    return outputPathSegments.join("/");
}

QString SpdrImportPrivate::getOperationStatusFromBool(bool status) const
{
    Q_Q(const SpdrImport);

    if (status) {
        return q->tr("succeeded");
    } else {
        return q->tr("failed");
    }
}

/*!
  Returns true if both the format and the file path are valid.
 */
bool SpdrImportPrivate::checkFormat(const QString &format) const
{
    Q_Q(const SpdrImport);

    bool result = true;

    // Reads both Unix and Windows paths
    QStringList pathSegments(format.split(QRegularExpression(mPathSeparatorRegularExpression),
                                          QString::SkipEmptyParts));

    foreach (const QString &segment, pathSegments) {
        int lessThanIndex = segment.indexOf("<");
        int greaterThanIndex = segment.indexOf(">");

        if (lessThanIndex == -1 && greaterThanIndex == -1) {
            // No template here
            continue;
        } else if (lessThanIndex == -1 || greaterThanIndex == -1) {
            result = false;
            q->log(q->tr("Missing tag enclosure: < or >: %1").arg(segment), Spdr::OnlyCritical);
            break;
        } else if (lessThanIndex > -1 && (lessThanIndex >= greaterThanIndex)) {
            result = false;
            q->log(q->tr("Date format tag is closed before it is opened: %1").arg(segment), Spdr::OnlyCritical);
            break;
        } else if (countOccurences(segment, '<') != countOccurences(segment, '>')) {
            result = false;
            q->log(q->tr("Too many date formatting tags: %1").arg(segment), Spdr::OnlyCritical);
            break;
        }
    }

    // TODO: check validity of the whole path

    return result;
}

int SpdrImportPrivate::countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount) const
{
    int result = 0;
    for (int i = 0; i < stringToSearchThrough.length(); i++) {
        if (stringToSearchThrough.at(i) == characterToCount) {
            result++;
        }
    }

    return result;
}

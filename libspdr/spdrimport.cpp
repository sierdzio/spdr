#include "spdrimport_p.h"

#include <QStringList>
#include <QRegularExpression>
#include <QDateTime>

SpdrImport::SpdrImport(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrImportPrivate(this))
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

QString SpdrImport::format() const
{
    Q_D(const SpdrImport);
    return d->mFormat;
}

/*!
  Can be used to set the format for the import dir, along with the path.

  Example format: ../myPhotos/<yyyy>/<MM>/<yyyy-MM-dd*>

  A star is being read as "anything goes" and can be used to import into some
  preexisting, named folders like "2014-01-27 Pics from today" with the following
  format "<yyyy-MM-dd*>".

  Accepted date formatting strings are the same as for QDateTime class and have
  to be put between "<" and ">" (otherwise they will be treated as part of the
  path).
 */
bool SpdrImport::setFormat(const QString &format)
{
    Q_D(SpdrImport);

    if (format != d->mFormat) {
        if (d->checkFormat(format)) {
            d->mFormat = format;
            emit formatChanged(format);
            return true;
        } else if (!d->mFormat.isEmpty()) {
            d->mFormat = QString::null;
            emit formatChanged(d->mFormat);
            return false;
        }
    }

    return false;
}

bool SpdrImport::import()
{
    Q_D(const SpdrImport);
    Q_UNUSED(d);

    bool result = true;

    return result;
}

bool SpdrImport::import(const QString &format)
{
    SpdrImport object;
    object.setFormat(format);
    return object.import();
}

SpdrImport::SpdrImport(SpdrImportPrivate &dd, QObject *parent) : SpdrBase(parent), d_ptr(&dd)
{
    Q_D(SpdrImport);
    Q_UNUSED(d);
}

/*!
  Returns true if both the format and the file path are valid.
 */
bool SpdrImportPrivate::checkFormat(const QString &format)
{
    Q_Q(SpdrImport);

    bool result = true;

    // Reads both Unix and Windows paths
    QStringList pathSegments(format.split(QRegularExpression("[\\\\]|[/]"), QString::SkipEmptyParts));

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

int SpdrImportPrivate::countOccurences(const QString &stringToSearchThrough, const QChar &characterToCount)
{
    int result = 0;
    for (int i = 0; i < stringToSearchThrough.length(); i++) {
        if (stringToSearchThrough.at(i) == characterToCount) {
            result++;
        }
    }

    return result;
}

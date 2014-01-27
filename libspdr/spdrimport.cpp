#include "spdrimport_p.h"

#include <QDateTime>

SpdrImport::SpdrImport(QObject *parent) : SpdrBase(parent), d_ptr(new SpdrImportPrivate)
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
        }
    }
}

bool SpdrImport::import()
{
    Q_D(const SpdrImport);
    Q_UNUSED(d);

    return true;
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

    // TODO: make sure to check both Unix and Windows path separators! (:/
    // yeah, this is boring)
    QStringList pathSegments(format.split("/"));

    foreach (const QString &segment, pathSegments) {
        int lessThanIndex = segment.indexOf("<");
        int greaterThanIndex = segment.indexOf(">");
        if (lessThanIndex == -1 && greaterThanIndex == -1) {
            // No template here
            continue;
        } else if (lessThanIndex > -1 && (lessThanIndex < greaterThanIndex)) {
            // Read the format string
        } else {
            result = false;
            // TODO: add logging
        }
    }

    // TODO: check validity of the whole path

    return result;
}

#include "spdrfiledata.h"

/*!
  Compares to files using most of available data.
  */
bool SpdrFileData::operator ==(const SpdrFileData &other) const
{
    if ((isValid == other.isValid) && (name == other.name) && (path == other.path)
            && (absoluteFilePath == other.absoluteFilePath)
            && (checksumMd5 == other.checksumMd5) && (checksumSha == other.checksumSha)
            && (creationDate == other.creationDate) && (size == other.size)) {
        return true;
    }

    return false;
}

/*!
  Returns a nicely formatted information about the file.
  */
QString SpdrFileData::toString() const
{
    QString result("Name: " + name + "\n");
    result += "Relative path: " + path + "\n";
    result += "Absolute file path: " + absoluteFilePath + "\n";
    result += "MD5: " + checksumMd5 + "\n";
    result += "SHA: " + checksumSha + "\n";
    result += "Created: " + creationDate.toString("dd-MM-yyyy") + "\n";
    result += "Size: " + QString::number(size);
    return result;
}

/*!
  Returns true if current file is equal to \a other. That means that the names,
  creation dates, checksums and sizes are all the same.
  */
bool SpdrFileData::isEqual(const SpdrFileData &other)
{
    if ((name == other.name) && (checksumMd5 == other.checksumMd5)
            && (checksumSha == other.checksumSha)
            && (size == other.size))
    {
        return true;
    }

    return false;
}

/*!
  Returns true if the current file and \a other are the same, but moved
  to some other place (and/ or renamed).
  */
bool SpdrFileData::isMoved(const SpdrFileData &other)
{
    if ((checksumMd5 == other.checksumMd5)
            && (checksumSha == other.checksumSha)
            && (size == other.size))
    {
        return true;
    }

    return false;
}

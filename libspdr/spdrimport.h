#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "SpdrBase"

#include <QString>
#include <QObject>

class SpdrImportPrivate;

/*!
  This class can be used to intelligently import a directory, dividing all files
  into neatly organised set of directories.

  For example, you may have a directory full of unsorted photos. Then, by importing
  that directory with this filter set: ../myPhotos/<yyyy>/<MM>/<yyyy-MM-dd>, it will
  copy all files to "myPhotos", and also make sure they are all put into separate
  directories based on the date they were taken.
 */
class SPDR_DLLSPEC SpdrImport : public SpdrBase {

    Q_OBJECT
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)

public:
    SpdrImport(QObject *parent = 0);

    Q_INVOKABLE QString format() const;
    Q_INVOKABLE bool setFormat(const QString &format);

    Q_INVOKABLE bool import();
    Q_INVOKABLE static bool import(const QString &format);

signals:
    void formatChanged(const QString &newFormat);

protected:
    SpdrImport(SpdrImportPrivate &dd, QObject *parent = 0);
    SpdrImportPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrImport)
};

#endif // SPDRIMPORT_H

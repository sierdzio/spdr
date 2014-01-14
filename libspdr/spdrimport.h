#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

class SpdrImportPrivate;

class SPDR_DLLSPEC SpdrImport : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)

public:
    SpdrImport(QObject *parent = 0);

    Q_INVOKABLE QString format() const;
    Q_INVOKABLE void setFormat(const QString &format);

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

#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

// TODO: migrate to PIMPL
class SPDR_DLLSPEC SpdrImport : public QObject {

    Q_OBJECT

public:
    SpdrImport(QObject *parent = 0);

    Q_PROPERTY(QString format MEMBER mFormat NOTIFY formatChanged)

    Q_INVOKABLE bool import();
    Q_INVOKABLE bool import(const QString &format);

signals:
    void formatChanged(const QString &newFormat);

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &d);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

#endif // SPDRIMPORT_H

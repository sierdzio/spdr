#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "SpdrBase"

#include <QString>
#include <QObject>

class SpdrImportPrivate;

class SPDR_DLLSPEC SpdrImport : public SpdrBase
{
    Q_OBJECT

public:
    SpdrImport(QObject *parent = 0);

    Q_INVOKABLE bool setOutputPath(const QString &newOutputPath);

    Q_INVOKABLE bool import() const;
    Q_INVOKABLE static bool import(const QString &inputPath, const QString &outputPath);

protected:
    SpdrImport(SpdrImportPrivate &dd, QObject *parent = 0);
    SpdrImportPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrImport)
};

#endif // SPDRIMPORT_H

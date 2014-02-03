#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "SpdrBase"

#include <QString>
#include <QObject>

class SpdrImportPrivate;

class SPDR_DLLSPEC SpdrImport : public SpdrBase
{
    Q_OBJECT
    Q_ENUMS(CopyMode)

public:
    /*!
      Determines what operation should Spdr perform on files it is operating on
     */
    enum CopyMode {
        Copy = 0, //!< Files will be copied (no changes in input paths)
        Move = 1 //!< Files will be moved (file may be deleted from input path, if the move operation is succesful)
    };

    SpdrImport(QObject *parent = 0);

    Q_INVOKABLE bool setOutputPath(const QString &newOutputPath);

    Q_INVOKABLE CopyMode copyMode() const;
    Q_INVOKABLE void setCopyMode(CopyMode newCopyMode);

    Q_INVOKABLE bool import() const;
    Q_INVOKABLE static bool import(const QString &inputPath, const QString &outputPath);

    Q_INVOKABLE static QString copyModeToString(CopyMode mode);

    static void registerMetatypes();

signals:
    void copyModeChanged(SpdrImport::CopyMode newCopyMode);

protected:
    SpdrImport(SpdrImportPrivate &dd, QObject *parent = 0);
    SpdrImportPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrImport)
};

#endif // SPDRIMPORT_H

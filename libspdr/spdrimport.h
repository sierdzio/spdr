#ifndef SPDRIMPORT_H
#define SPDRIMPORT_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

class SpdrImportPrivate;

class SPDR_DLLSPEC SpdrImport : public QObject {

    Q_OBJECT
    //Q_PROPERTY(Spdr::CopyMode copyMode READ copyMode WRITE setCopyMode NOTIFY copyModeChanged)
    //Q_PROPERTY(Spdr::UpdateMode updateMode READ updateMode WRITE setUpdateMode NOTIFY updateModeChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat NOTIFY formatChanged)

public:
    SpdrImport(QObject *parent = 0);

    Q_INVOKABLE Spdr::CopyMode copyMode() const;
    Q_INVOKABLE void setCopyMode(Spdr::CopyMode newCopyMode);

    Q_INVOKABLE Spdr::UpdateMode updateMode() const;
    Q_INVOKABLE void setUpdateMode(Spdr::UpdateMode newUpdateMode);

    Q_INVOKABLE Spdr::LogLevel logLevel() const;
    Q_INVOKABLE void setLogLevel(Spdr::LogLevel newLevel);
    Q_INVOKABLE void setLogFile(const QString &logFilePath);

    Q_INVOKABLE QString format() const;
    Q_INVOKABLE void setFormat(const QString &format);

    Q_INVOKABLE bool import();
    Q_INVOKABLE static bool import(const QString &format);

signals:
    void copyModeChanged(Spdr::CopyMode newCopyMode);
    void updateModeChanged(Spdr::UpdateMode newUpdateMode);
    void formatChanged(const QString &newFormat);

protected:
    SpdrImport(SpdrImportPrivate &dd, QObject *parent = 0);
    SpdrImportPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrImport)
};

#endif // SPDRIMPORT_H

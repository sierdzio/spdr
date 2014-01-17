#ifndef SPDRSYNCHRONIZE_H
#define SPDRSYNCHRONIZE_H

#include "spdrglobal.h"

#include <QObject>

class SpdrSynchronizePrivate;

class SPDR_DLLSPEC SpdrSynchronize : public QObject {

    Q_OBJECT
    Q_FLAGS(SynchronizationOptions)
    Q_PROPERTY(int split READ split WRITE setSplit NOTIFY splitChanged)
    //Q_PROPERTY(Spdr::CopyMode copyMode READ copyMode WRITE setCopyMode NOTIFY copyModeChanged)
    //Q_PROPERTY(Spdr::UpdateMode updateMode READ updateMode WRITE setUpdateMode NOTIFY updateModeChanged)

public:
    SpdrSynchronize(QObject *parent = 0);

    enum SynchronizationOption {
        None                   = 0x0000,
        Bidirectional          = 0x0001,
        Cache                  = 0x0002,
        Simulate               = 0x0004,
        RemoveEmptyDirectories = 0x0008,
        RemoveMissingFiles     = 0x0010,
        DeepSearch             = 0x0020
    };
    Q_DECLARE_FLAGS(SynchronizationOptions, SynchronizationOption)

    Q_INVOKABLE Spdr::CopyMode copyMode() const;
    Q_INVOKABLE void setCopyMode(Spdr::CopyMode newCopyMode);

    Q_INVOKABLE Spdr::UpdateMode updateMode() const;
    Q_INVOKABLE void setUpdateMode(Spdr::UpdateMode newUpdateMode);

    Q_INVOKABLE Spdr::LogLevel logLevel() const;
    Q_INVOKABLE void setLogLevel(Spdr::LogLevel newLevel);
    Q_INVOKABLE void setLogFile(const QString &logFilePath);

    Q_INVOKABLE SynchronizationOptions options() const;
    Q_INVOKABLE void setOptions(SynchronizationOptions options);

    Q_INVOKABLE int split() const;
    Q_INVOKABLE void setSplit(uint split);

    Q_INVOKABLE bool synchronize();

signals:
    void copyModeChanged(Spdr::CopyMode newCopyMode);
    void updateModeChanged(Spdr::UpdateMode newUpdateMode);
    void optionsChanged(SynchronizationOptions newOptions);
    void splitChanged(int newSplit);

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent = 0);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SpdrSynchronize::SynchronizationOptions)

#endif // SPDRSYNCHRONIZE_H

#ifndef SPDRSYNCHRONIZE_H
#define SPDRSYNCHRONIZE_H

#include "SpdrBase"

#include <QObject>

class SpdrSynchronizePrivate;

class SPDR_DLLSPEC SpdrSynchronize : public SpdrBase {

    Q_OBJECT
    Q_FLAGS(SynchronizationOptions)
    Q_PROPERTY(int split READ split WRITE setSplit NOTIFY splitChanged)

public:
    SpdrSynchronize(QObject *parent = 0);

    // TODO: add toString() method for Options enum
    enum SynchronizationOption {
        None                   = 0x0000,
        Bidirectional          = 0x0001,
        Cache                  = 0x0002,
        RemoveEmptyDirectories = 0x0004,
        RemoveMissingFiles     = 0x0008,
        DeepSearch             = 0x0010
    };
    Q_DECLARE_FLAGS(SynchronizationOptions, SynchronizationOption)

    Q_INVOKABLE SynchronizationOptions options() const;
    Q_INVOKABLE void setOptions(SynchronizationOptions options);

    Q_INVOKABLE int split() const;
    Q_INVOKABLE void setSplit(uint split);

    Q_INVOKABLE bool synchronize() const;

    static void registerMetatypes();
    static QString synchronizationOptionsToString(SynchronizationOptions optionSet);

signals:
    void optionsChanged(SpdrSynchronize::SynchronizationOptions newOptions);
    void splitChanged(int newSplit);

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent = 0);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SpdrSynchronize::SynchronizationOptions)

#endif // SPDRSYNCHRONIZE_H

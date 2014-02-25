#ifndef SPDRSYNCHRONIZE_H
#define SPDRSYNCHRONIZE_H

#include "SpdrBase"
#include "spdrfiledata.h"

#include <QString>
#include <QObject>

class SpdrSynchronizePrivate;

class SPDR_DLLSPEC SpdrSynchronize : public SpdrBase {

    Q_OBJECT
    Q_FLAGS(SynchronizationOptions)

public:
    SpdrSynchronize(QObject *parent = 0);

    enum SynchronizationOption {
        None                   = 0x0000,
        RemoveEmptyDirectories = 0x0001,
        RemoveMissingFiles     = 0x0002,
        DeepSearch             = 0x0004
    };
    Q_DECLARE_FLAGS(SynchronizationOptions, SynchronizationOption)

    Q_INVOKABLE SynchronizationOptions options() const;
    Q_INVOKABLE void setOptions(SynchronizationOptions options);
    static QString synchronizationOptionsToString(SynchronizationOptions optionSet);

    static void registerMetatypes();

public slots:
    bool synchronize() const;

signals:
    void optionsChanged(SpdrSynchronize::SynchronizationOptions newOptions) const;

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent = 0);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SpdrSynchronize::SynchronizationOptions)

#endif // SPDRSYNCHRONIZE_H

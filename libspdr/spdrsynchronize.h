#ifndef SPDRSYNCHRONIZE_H
#define SPDRSYNCHRONIZE_H

#include "spdrglobal.h"

#include <QObject>

class SpdrSynchronizePrivate;

class SPDR_DLLSPEC SpdrSynchronize : public QObject {

    Q_OBJECT
    Q_PROPERTY(int split READ split WRITE setSplit NOTIFY splitChanged)
    Q_FLAGS(SynchronizationOptions)

public:
    SpdrSynchronize(QObject *parent = 0);

    enum SynchronizationOption {
    };
    Q_DECLARE_FLAGS(SynchronizationOptions, SynchronizationOption)

    Q_INVOKABLE int split() const;
    Q_INVOKABLE void setSplit(uint split);

signals:
    void splitChanged(int newSplit);

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent = 0);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SpdrSynchronize::SynchronizationOptions)

#endif // SPDRSYNCHRONIZE_H

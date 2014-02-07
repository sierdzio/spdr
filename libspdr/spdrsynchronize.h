#ifndef SPDRSYNCHRONIZE_H
#define SPDRSYNCHRONIZE_H

#include "SpdrBase"

#include <QString>
#include <QObject>

class SpdrSynchronizePrivate;

class SPDR_DLLSPEC SpdrSynchronize : public SpdrBase {

    Q_OBJECT
    Q_FLAGS(SynchronizationOptions)
    Q_PROPERTY(int split READ split WRITE setSplit NOTIFY splitChanged)

public:
    SpdrSynchronize(QObject *parent = 0);

    /*!
      Contains additional flags that can be passed to synchronization algorithm.
     */
    enum SynchronizationOption {
        None                   = 0x0000, //!< No additional options will be used
        RemoveEmptyDirectories = 0x0001, /*!< If syncing makes a directory empty,
                                           it will be removed */
        RemoveMissingFiles     = 0x0002, /*!< If some files are missing in input,
                                           they will also be removed from output */
        DeepSearch             = 0x0004 /*!< Spdr will search for existing files
                                           using all available methods: file name,
                                           file position, file's data hash, file diff */
    };
    Q_DECLARE_FLAGS(SynchronizationOptions, SynchronizationOption)

    Q_INVOKABLE SynchronizationOptions options() const;
    Q_INVOKABLE void setOptions(SynchronizationOptions options);
    static QString synchronizationOptionsToString(SynchronizationOptions optionSet);

    Q_INVOKABLE int split() const;
    Q_INVOKABLE void setSplit(uint split);

    static void registerMetatypes();

public slots:
    bool synchronize() const;

signals:
    void optionsChanged(SpdrSynchronize::SynchronizationOptions newOptions) const;
    void splitChanged(int newSplit) const;

protected:
    SpdrSynchronize(SpdrSynchronizePrivate &dd, QObject *parent = 0);
    SpdrSynchronizePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrSynchronize)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SpdrSynchronize::SynchronizationOptions)

#endif // SPDRSYNCHRONIZE_H

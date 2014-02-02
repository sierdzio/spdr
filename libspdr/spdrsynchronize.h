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
        Bidirectional          = 0x0001, /*!< Sync will be performed in both ways
                                           (input and output swap after
                                           first synchronization) */
        Cache                  = 0x0002, /*!< Spdr will cache output folder structure,
                                           thus improving performance (at the cost
                                           of using more memory) */
        RemoveEmptyDirectories = 0x0004, /*!< If syncing makes a directory empty,
                                           it will be removed */
        RemoveMissingFiles     = 0x0008, /*!< If some files are missing in input,
                                           they will also be removed from output */
        DeepSearch             = 0x0010 /*!< Spdr will search for existing files
                                           using all available methods: file name,
                                           file position, file's data hash, file diff */
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

#ifndef SPDRDLLSPEC_H
#define SPDRDLLSPEC_H

//#include <qglobal.h>
#include <QObject>

#ifdef LIBSPDR
#define SPDR_DLLSPEC  Q_DECL_EXPORT
#else
#define SPDR_DLLSPEC Q_DECL_IMPORT
#endif

class Spdr : public QObject {
    Q_OBJECT
    Q_ENUMS(UpdateMode CopyMode LogLevel)

public:
    /*!
      Determines what Spdr should do when the output file already exists.
     */
    enum UpdateMode {
        Ask       = 0, //! Spdr will ask what should be done every time
        Overwrite = 1, //! Spdr will try to overwrite the output file
        Ignore    = 2 //! Spdr will ignore (skip) that file
    };

    /*!
      Determines what operation should Spdr perform on files it is operating on
     */
    enum CopyMode {
        Copy       = 0, //! Files will be copied (no changes in input paths)
        Move       = 1 //! Files will be moved (file may be deleted from input path, if the move operation is succesful)
    };

    /*!
      Specifies the desired input level.
     */
    enum LogLevel {
        NoLogging = 0, //! No logging information will be printed
        OnlyCritical = 1, //! Only critical errors will be printed
        OnlyErrors = 2, //! All errors will be printed
        MildLogging = 3, //! Some important log information will be printed
        MediumLogging = 4, //! Most interesting pieces of information will be printed
        LogEverything = 5, //! All log messages will be printed
        Debug = LogEverything //! Synonym for LogEverything
    };

    /*!
      This method can be used to register Spdr enums with the Meta Object system
     */
    static void registerMetatypes()
    {
        qRegisterMetaType<Spdr::UpdateMode>("Spdr::UpdateMode");
        qRegisterMetaType<Spdr::CopyMode>("Spdr::CopyMode");
        qRegisterMetaType<Spdr::LogLevel>("Spdr::LogLevel");
    }

private:
    Spdr(QObject *parent = 0) : QObject(parent) {}
    ~Spdr() {}
};

#endif // SPDRDLLSPEC_H

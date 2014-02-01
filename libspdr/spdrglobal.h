#ifndef SPDRDLLSPEC_H
#define SPDRDLLSPEC_H

#include <QString>
#include <QObject>

#ifdef LIBSPDR
#define SPDR_DLLSPEC  Q_DECL_EXPORT
#else
#define SPDR_DLLSPEC Q_DECL_IMPORT
#endif

/*!
  Defines various enums and static functions that are needed in all other code
  using Spdr.
 */
class Spdr : public QObject {
    Q_OBJECT
    Q_ENUMS(UpdateMode CopyMode LogLevel)

public:
    // TODO: add toString() method for all enums!

    /*!
      Determines what Spdr should do when the output file already exists.
     */
    enum UpdateMode {
        Ask       = 0, //!< Spdr will ask what should be done every time
        Overwrite = 1, //!< Spdr will try to overwrite the output file
        Ignore    = 2 //!< Spdr will ignore (skip) that file
    };

    /*!
      Determines what operation should Spdr perform on files it is operating on
     */
    enum CopyMode {
        Copy = 0, //!< Files will be copied (no changes in input paths)
        Move = 1 //!< Files will be moved (file may be deleted from input path, if the move operation is succesful)
    };

    /*!
      Specifies the desired input level.
     */
    enum LogLevel {
        NoLogging = 0, //!< No logging information will be printed
        Critical = 1, //!< Only critical errors will be printed
        Error = 2, //!< All errors will be printed
        MildLogging = 3, //!< Some important log information will be printed
        MediumLogging = 4, //!< Most interesting pieces of information will be printed
        ExcessiveLogging = 5, //! Use this only when you need to be flooded with messages
        LogEverything = 6, //!< All log messages will be printed
        Debug = LogEverything //!< Synonym for LogEverything
    };

    static QString updateModeToString(UpdateMode mode)
    {
        QString result;
        if (mode == Ask) {
            result = "Ask";
        } else if (mode == Overwrite) {
            result = "Overwrite";
        } else if (mode == Ignore) {
            result = "Ignore";
        }

        return result;
    }

    static QString copyModeToString(CopyMode mode)
    {
        QString result;
        if (mode == Copy) {
            result = "Copy";
        } else if (mode == Move) {
            result = "Move";
        }

        return result;
    }

    static QString logLevelToString(LogLevel level)
    {
        QString result;
        if (level == NoLogging) {
            result = "NoLogging";
        } else if (level == Critical) {
            result = "OnlyCritical";
        } else if (level == Error) {
            result = "OnlyErrors";
        } else if (level == MildLogging) {
            result = "MildLogging";
        } else if (level == MediumLogging) {
            result = "MediumLogging";
        } else if (level == ExcessiveLogging) {
            result = "ExcessiveLogging";
        } else if (level == LogEverything || level == Debug) {
            result = "LogEverything";
        }

        return result;
    }

    /*!
      This method can be used to register Spdr enums with the Meta Object system.

      Useful when one needs to use those enums in Qt tests.
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

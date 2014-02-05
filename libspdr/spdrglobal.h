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
    Q_ENUMS(UpdateMode LogLevel)

public:
    /*!
      Determines what Spdr should do when the output file already exists.
     */
    enum UpdateMode {
        Ask       = 0, //!< Spdr will ask what should be done every time
        Overwrite = 1, //!< Spdr will try to overwrite the output file
        Ignore    = 2 //!< Spdr will ignore (skip) that file
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

    /*!
      Returns a string representation of given \a mode. Useful in debugging/ logging.
     */
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

    /*!
      Returns a string representation of given \a level of logging.
     */
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
      This is just an internal convenience method used for returning translatable
      operation status messages.
     */
    static QString getOperationStatusFromBool(bool status)
    {
        if (status) {
            return QObject::tr("success");
        } else {
            return QObject::tr("failure");
        }
    }

    /*!
      This method can be used to register Spdr enums with the Meta Object system.

      Useful when one needs to use those enums in Qt tests.
     */
    static void registerMetatypes()
    {
        qRegisterMetaType<Spdr::UpdateMode>("Spdr::UpdateMode");
        qRegisterMetaType<Spdr::LogLevel>("Spdr::LogLevel");
    }

private:
    Spdr(QObject *parent = 0) : QObject(parent) {}
    ~Spdr() {}
};

#endif // SPDRDLLSPEC_H

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
    enum UpdateMode {
        Ask       = 0,
        Overwrite = 1,
        Ignore    = 2
    };

    enum CopyMode {
        Copy       = 0,
        Move       = 1,
        AlwaysMove = 2
    };

    enum LogLevel {
        NoLogging = 0,
        OnlyCritical = 1,
        OnlyErrors = 2,
        MildLogging = 3,
        MediumLogging = 4,
        LogEverything = 5
    };

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

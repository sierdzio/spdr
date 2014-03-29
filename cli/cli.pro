QT = core

TARGET = spdr-cli

TEMPLATE = app

include(../libraryIncludes.pri)
include(../definitions.pri)

TRANSLATIONS += locale/spdr_cli_pl.ts

QMAKE_POST_LINK = lupdate cli.pro && lrelease cli.pro

SOURCES += \
    main.cpp \
    spdrcliparser.cpp \
    spdrclirunner.cpp

mac {
    QMAKE_POST_LINK = ln -f $${BUILDDIR}/../spdr/libspdr.0.dylib $${BUILDDIR}/$${TARGET}.app/Contents/MacOS/
}

HEADERS += \
    spdrcliparser.h \
    spdrclirunner.h \
    spdrclioptions.h

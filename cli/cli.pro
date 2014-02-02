QT = core

TARGET = spdr-cli

TEMPLATE = app

include(../libraryIncludes.pri)
include(../definitions.pri)

SOURCES += \
    main.cpp \
    spdrcliparser.cpp

mac {
    QMAKE_POST_LINK = ln -f $${BUILDDIR}/../spdr/libspdr.0.dylib $${BUILDDIR}/$${TARGET}.app/Contents/MacOS/
}

HEADERS += \
    spdrcliparser.h

QT = core gui widgets

TARGET = spdr-gui

TEMPLATE = app

include(../libraryIncludes.pri)
include(../definitions.pri)

INCLUDEPATH += src

SOURCES += \
    main.cpp

TRANSLATIONS += locale/spdr_pl.ts

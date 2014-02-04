QT = core gui widgets

TARGET = spdr-gui

TEMPLATE = app

include(../libraryIncludes.pri)
include(../definitions.pri)

INCLUDEPATH += src

SOURCES += \
    main.cpp \
    spdrguimainwindow.cpp

TRANSLATIONS += locale/spdr_pl.ts

FORMS += \
    spdrguimainwindow.ui

HEADERS += \
    spdrguimainwindow.h

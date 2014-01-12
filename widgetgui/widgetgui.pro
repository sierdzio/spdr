QT = core gui widgets

TARGET = spdr-gui

TEMPLATE = app

include(../definitions.pri)

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/pdmainwindow.cpp \
    src/pdsettingsdialog.cpp

HEADERS += \
    src/pdmainwindow.h \
    src/pdsettingsdialog.h

FORMS += \
    src/pdmainwindow.ui \
    src/pdsettingsdialog.ui

TRANSLATIONS += locale/spdr_pl.ts

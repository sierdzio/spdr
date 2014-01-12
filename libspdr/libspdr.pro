QT = core

TARGET = spdr

TEMPLATE = lib

include(../definitions.pri)

INCLUDEPATH += src

SOURCES += \
    src/pddownloader.cpp \
    src/pdfolderhandlers.cpp

HEADERS += \
    src/pddownloader.h \
    src/pdfolderhandlers.h

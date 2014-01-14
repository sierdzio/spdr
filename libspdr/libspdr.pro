QT = core

TARGET = spdr

TEMPLATE = lib

include(../definitions.pri)

DEFINES += LIBSPDR

VERSION = 0.0.1

static {
  message("Library built in static mode")
  CONFIG += staticlib
} else {
#  message("Library building in shared mode")
}

INCLUDEPATH += src

SOURCES += \
    spdrimport.cpp \
    spdrsynchronize.cpp

HEADERS += \
    Spdr \
    SpdrImport \
    SpdrSynchronize \
    spdrglobal.h \
    spdrimport.h \
    spdrimport_p.h \
    spdrsynchronize.h \
    spdrsynchronize_p.h
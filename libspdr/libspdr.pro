QT = core

TARGET = spdr

TEMPLATE = lib

include(../definitions.pri)

DEFINES += LIBSPDR

VERSION = 0.1.0

static {
  message("Library built in static mode")
  CONFIG += staticlib
} else {
#  message("Library building in shared mode")
}

INCLUDEPATH += src

SOURCES += \
    spdrimport.cpp \
    spdrsynchronize.cpp \
    spdrlog.cpp \
    spdrbase.cpp

HEADERS += \
    Spdr \
    SpdrLog \
    SpdrBase \
    SpdrImport \
    SpdrSynchronize \
    spdrglobal.h \
    spdrimport.h \
    spdrimport_p.h \
    spdrsynchronize.h \
    spdrsynchronize_p.h \
    spdrlog.h \
    spdrbase.h \
    spdrbase_p.h \
    spdrlog_p.h

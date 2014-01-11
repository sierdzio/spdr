# -------------------------------------------------
# Project created by QtCreator 2009-09-12T10:39:32
# Distributed under WTFPL license
# Author: Tomasz Siekierda a.k.a sierdzio
# sierdzio@gmail.com
# http://www.sierdzio.com
# http://www.sierdzio.deviantart.com
# -------------------------------------------------

QT = core gui

TARGET = spdar

BUILDDIR = $$PWD/build/

DESTDIR = $${BUILDDIR}
OBJECTS_DIR = $${BUILDDIR}
MOC_DIR = $${BUILDDIR}
RCC_DIR = $${BUILDDIR}
UI_DIR = $${BUILDDIR}

TEMPLATE = app

SOURCES += src/main.cpp \
    src/pdmainwindow.cpp \
    src/pddownloader.cpp \
    src/pdredistributor.cpp \
    src/pdfolderhandlers.cpp \
    src/pdsettingsdialog.cpp

HEADERS += include/pdmainwindow.h \
    include/pddownloader.h \
    include/pdredistributor.h \
    include/pdfolderhandlers.h \
    include/pdsettingsdialog.h

FORMS += ui/pdmainwindow.ui \
    ui/pdsettingsdialog.ui

TRANSLATIONS += ./locale/sPDaR_pl.ts

OTHER_FILES += \
    AUTHORS.txt \
    COPYING.txt \
    DEVELOPER_README.txt \
    LICENSE.txt \
    README.txt \
    CHANGELOG.txt

# -------------------------------------------------
# Project created by QtCreator 2009-09-12T10:39:32
# Distributed under GNU GPL 3 licence
# Author: Tomasz Siekierda a.k.a sierdzio
# sierdzio@gmail.com
# http://www.sierdzio.com
# http://www.sierdzio.deviantart.com
# -------------------------------------------------

TARGET = photoDownloader

TEMPLATE = app

SOURCES += sources/main.cpp \
    sources/pdmainwindow.cpp \
    sources/pddownloader.cpp \
    sources/pdredistributor.cpp \
    sources/pdfolderhandlers.cpp \
    sources/pdsettingsdialog.cpp

HEADERS += headers/pdmainwindow.h \
    headers/pddownloader.h \
    headers/pdredistributor.h \
    headers/pdfolderhandlers.h \
    headers/pdsettingsdialog.h

FORMS += ui/pdmainwindow.ui \
    ui/pdsettingsdialog.ui

TRANSLATIONS += ./locale/sPDaR_pl.ts

OTHER_FILES += \
    doc/AUTHORS.txt \
    doc/COPYING.txt \
    doc/DEVELOPER_README.txt \
    doc/LICENCE.txt \
    doc/README.txt \
    doc/CHANGELOG.txt

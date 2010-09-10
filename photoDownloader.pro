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
SOURCES += main.cpp \
    pdmainwindow.cpp \
    pddownloader.cpp \
    pdredistributor.cpp \
    pdfolderhandlers.cpp \
    pdsettingsdialog.cpp
HEADERS += pdmainwindow.h \
    pddownloader.h \
    pdredistributor.h \
    pdfolderhandlers.h \
    pdsettingsdialog.h
FORMS += pdmainwindow.ui \
    pdsettingsdialog.ui
TRANSLATIONS += ./locale/sPDaR_pl.ts

OTHER_FILES += \
    AUTHORS.txt \
    COPYING.txt \
    DEVELOPER_README.txt \
    LICENCE.txt \
    README.txt \
    CHANGELOG.txt

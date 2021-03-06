QT = core gui widgets

TARGET = spdr-gui

TEMPLATE = app

include(../libraryIncludes.pri)
include(../definitions.pri)

INCLUDEPATH += src

SOURCES += \
    main.cpp \
    spdrguimainwindow.cpp \
    spdrguibasicinputform.cpp \
    spdrguilineedit.cpp

TRANSLATIONS += locale/spdr_widgetgui_pl.ts

QMAKE_POST_LINK = lupdate widgetgui.pro && lrelease widgetgui.pro

FORMS += \
    spdrguimainwindow.ui \
    spdrguibasicinputform.ui

HEADERS += \
    spdrguimainwindow.h \
    spdrguibasicinputform.h \
    spdrguilineedit.h \
    spdrguitags.h

mac {
    QMAKE_POST_LINK = ln -f $${BUILDDIR}/../spdr/libspdr.0.dylib $${BUILDDIR}/$${TARGET}.app/Contents/MacOS/
}

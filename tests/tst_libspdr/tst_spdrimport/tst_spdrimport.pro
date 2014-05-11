QT = core testlib

CONFIG += testcase

include(../../../libraryIncludes.pri)
include(../../../definitions.pri)
include(../testincludes.pri)

#HEADERS += ../testhelpers.h
SOURCES += tst_spdrimport.cpp

mac {
    QMAKE_POST_LINK = ln -f $${BUILDDIR}/../spdr/libspdr.0.dylib $${BUILDDIR}/$${TARGET}.app/Contents/MacOS/
}

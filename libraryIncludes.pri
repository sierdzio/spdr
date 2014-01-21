win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build/spdr -lspdr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/spdr -lspdrd
else:unix: LIBS += -L$$PWD/build/spdr -lspdr

INCLUDEPATH += $$PWD/libspdr
DEPENDPATH += $$PWD/libspdr

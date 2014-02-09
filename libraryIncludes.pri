win32:CONFIG(release, debug|release): LIBS += -L$$PWD/build/spdr -lspdr0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/build/spdr -lspdr0
else:unix: LIBS += -L$$PWD/build/spdr -lspdr

INCLUDEPATH += $$PWD/libspdr
DEPENDPATH += $$PWD/libspdr

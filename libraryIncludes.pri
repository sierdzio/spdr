win32:CONFIG(release, debug|release): LIBS += -L$$PWD/bin -lspdr0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/bin -lspdr0
else:unix: LIBS += -L$$PWD/bin -lspdr

INCLUDEPATH += $$PWD/libspdr
DEPENDPATH += $$PWD/libspdr

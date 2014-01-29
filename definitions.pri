lessThan(QT_MAJOR_VERSION, 5): message("Spdr requires Qt 5.2 as the minimal version")

BUILDDIR = $$PWD/build/$$TARGET

DESTDIR = $${BUILDDIR}
OBJECTS_DIR = $${BUILDDIR}
MOC_DIR = $${BUILDDIR}
RCC_DIR = $${BUILDDIR}
UI_DIR = $${BUILDDIR}

#include <QApplication>
#include <QTranslator>

#include "spdrguimainwindow.h"

/*!
  \addtogroup wigetgui Spdr Widget GUI

  \brief widgetgui is a GUI application offering full functionality of the Spdr
  library.
  */

/*!
  \ingroup widgetgui

  Application's main function. Runs spdr dialog, optionally fires
  the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Spdr Widget GUI");
    app.setApplicationVersion("0.1.0");

    /*
    QTranslator translator;
    translator.load(app.applicationDirPath() + "/libspdr_pl");
    translator.load(app.applicationDirPath() + "/spdr_widgetgui_pl");
    app.installTranslator(&translator);
    */

    SpdrGuiMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

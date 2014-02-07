#include <QApplication>

#include "spdrguimainwindow.h"

/*
  Application's main function. Runs spdr dialog, optionally fires
  the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Spdr Widget GUI");
    app.setApplicationVersion("0.1.0");

    SpdrGuiMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

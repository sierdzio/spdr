#include <QApplication>

#include "spdrguimainwindow.h"

/*
  Application's main function. Runs spdr dialog, optionally fires
  the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SpdrGuiMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

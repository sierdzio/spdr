#include <QtGui/QApplication>
#include "../headers/pdmainwindow.h"

/*
    Application's main function. Runs sPDaR dialog, optionally fires
    the translator.
  */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create translator:
    QString locale = QLocale::system().name();
//    QTranslator sPDaRTranslator;
//    sPDaRTranslator.load("locale/sPDaR_pl");// + locale);
//    a.installTranslator(&sPDaRTranslator);

    PDMainWindow w;
    w.show();
    return a.exec();
}

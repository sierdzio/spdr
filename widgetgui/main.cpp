//#include "pdmainwindow.h"

//#include <QString>
//#include <QLocale>
#include <QApplication>
/*
    Application's main function. Runs sPDaR dialog, optionally fires
    the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create translator:
    //QString locale = QLocale::system().name();
//    QTranslator sPDaRTranslator;
//    sPDaRTranslator.load("locale/sPDaR_pl");// + locale);
//    a.installTranslator(&sPDaRTranslator);

    //PDMainWindow w;
    //w.show();
    //return a.exec();

    return 0;
}

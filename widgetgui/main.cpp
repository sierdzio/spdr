
#include <QApplication>

/*
  Application's main function. Runs spdr dialog, optionally fires
  the translator.
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    return app.exec();
}

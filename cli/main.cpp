#include <QCoreApplication>

#include "spdrcliparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("Spdr command line");
    app.setApplicationVersion("0.0.1");

    SpdrCliParser parser;
    parser.parse();

    return app.exec();
}

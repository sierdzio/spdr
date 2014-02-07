#include <QCoreApplication>
#include <QTimer>

#include "spdrcliparser.h"
#include "spdrclirunner.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("Spdr Command Line Tool");
    app.setApplicationVersion("0.1.0");

    SpdrCliParser parser;
    parser.parse();

    SpdrCliRunner runner;
    runner.setOptions(parser.options);

    QTimer::singleShot(0, &runner, SLOT(performActions()));

    return app.exec();
}

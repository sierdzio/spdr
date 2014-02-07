#include <QCoreApplication>
#include <QTimer>

#include "spdrcliparser.h"
#include "spdrclirunner.h"

/*!
  \addtogroup cli Spdr Command Line Tool

  \brief spdr-cli is a handy tool offering the whole functionality of Spdr from
  the command line.

  Run spdr-cli --help to get more info on how to use it.
  */

/*!
  \ingroup cli

  The main routine: reads from command line and runs the operations that the user
  has requested.
  */
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

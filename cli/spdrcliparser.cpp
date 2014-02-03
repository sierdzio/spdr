#include "spdrcliparser.h"

#include <QStringList>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

SpdrCliParser::SpdrCliParser(QObject *parent) : QObject(parent)
{
    options.isImport = false;
    options.isSimulation = false;

    options.updateMode = Spdr::Overwrite;
    options.logLevel = Spdr::MildLogging;
    options.copyMode = SpdrImport::Copy;
    options.synchronizationOptions = SpdrSynchronize::Cache
            | SpdrSynchronize::RemoveEmptyDirectories
            | SpdrSynchronize::RemoveMissingFiles;
}

bool SpdrCliParser::parse()
{
    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Synchronize even the largest directory structures easily"));
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("input", tr("Input folder, usually containing updated data"));
    parser.addPositionalArgument("output", tr("Output folder, usually containing old data. When Import operation is performed, the path can contain QDateTime-style time and date tags enclosed in < and >, as well as star * wildcard matching. See the documentation for more information"));

    QCommandLineOption importOption(QStringList() << "i" << "import",
                                       tr("Spdr will perform import operation: it will copy all files from input according to formatting tags specified in output directory path. See the documentation for more information"));
    parser.addOption(importOption);

    QCommandLineOption synchronizeOption(QStringList() << "s" << "synchronize",
                                       tr("(default) Spdr will perform file synchronization. See the documentation for more information"));
    parser.addOption(synchronizeOption);

    QCommandLineOption logFileOption(QStringList() << "l" << "log",
                                     tr("Log file location. If empty/ not specified, Spdr will print to stdout"),
                                     tr("path"));
    parser.addOption(logFileOption);

    QCommandLineOption logLevelOption("log-level",
                                      tr("Specifies how many log messages will be shown. Accepted values are 0 (no logging) to 6 (log everything, including debug output). See the documentation for more information."),
                                      tr("number"), "3");
    parser.addOption(logLevelOption);

    QCommandLineOption moveOption(QStringList() << "m" << "move",
                                  tr("Files will be moved from input to output (input will change!). By default, Spdr copies the files (leavign input intact)"));
    parser.addOption(moveOption);

    QCommandLineOption simulateOption("simulate",
                                  tr("Spdr will simulate all the actions, without actually doing any changes to the file system"));
    parser.addOption(simulateOption);

//    QCommandLineOption noCacheOption(QStringList() << "c" << "no-cache",
//                                  tr("caching will not be used"));
//    parser.addOption(noCacheOption);

    parser.process(QCoreApplication::instance()->arguments());

    // Read the results and assign them
    QStringList inout(parser.positionalArguments());

    if (inout.count() < 2) {
        parser.showHelp(1);
        return false;
    }

    options.inputPath = inout.at(0);
    options.outputPath = inout.at(1);

    if (parser.isSet(logLevelOption)) {
        bool isOk = true;
        int result = parser.value(logLevelOption).toInt(&isOk);

        if (!isOk) {
            parser.showHelp(1);
            return false;
        }

        options.logLevel = (Spdr::LogLevel) result;
    }

    if (parser.isSet(logFileOption)) {
        options.logFile = parser.value(logFileOption);
    }

    options.isImport = parser.isSet(importOption);
    options.isSimulation = parser.isSet(simulateOption);

    if (parser.isSet(moveOption)) {
        options.copyMode = SpdrImport::Move;
    }

    //updateMode;
    //synchronizationOptions;

    return true;
}

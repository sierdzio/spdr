#include "spdrclirunner.h"

#include "SpdrImport"
#include "SpdrSynchronize"

#include <QCoreApplication>

SpdrCliRunner::SpdrCliRunner(QObject *parent) : QObject(parent)
{
}

void SpdrCliRunner::setOptions(SpdrCliOptions options)
{
    mOptions = options;
}

void SpdrCliRunner::performActions()
{
    bool result = false;
    if (mOptions.isImport) {
        SpdrImport import(this);
        import.setInputPath(mOptions.inputPath);
        import.setOutputPath(mOptions.outputPath);
        import.setLogFile(mOptions.logFile);
        import.setLogLevel(mOptions.logLevel);
        import.setSimulate(mOptions.isSimulation);
        import.setCopyMode(mOptions.copyMode);
        import.setUpdateMode(mOptions.updateMode);
        result = import.import();
    } else {
        SpdrSynchronize synchronize(this);
        synchronize.setInputPath(mOptions.inputPath);
        synchronize.setOutputPath(mOptions.outputPath);
        synchronize.setLogFile(mOptions.logFile);
        synchronize.setLogLevel(mOptions.logLevel);
        synchronize.setSimulate(mOptions.isSimulation);
        synchronize.setUpdateMode(mOptions.updateMode);
        synchronize.setOptions(mOptions.synchronizationOptions);
        result = synchronize.synchronize();
    }

    QCoreApplication::instance()->exit(!result);
}

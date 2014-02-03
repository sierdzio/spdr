#ifndef SPDRCLIOPTIONS_H
#define SPDRCLIOPTIONS_H

#include "SpdrBase"
#include "SpdrImport"
#include "SpdrSynchronize"

#include <QString>

class SpdrCliOptions
{
public:
    bool isImport;
    bool isSimulation;

    Spdr::UpdateMode updateMode;
    Spdr::LogLevel logLevel;
    SpdrImport::CopyMode copyMode;
    SpdrSynchronize::SynchronizationOptions synchronizationOptions;

    QString logFile;
    QString inputPath;
    QString outputPath;
};

#endif // SPDRCLIOPTIONS_H

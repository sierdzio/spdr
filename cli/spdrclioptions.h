#ifndef SPDRCLIOPTIONS_H
#define SPDRCLIOPTIONS_H

#include "SpdrBase"
#include "SpdrSynchronize"

#include <QString>

class SpdrCliOptions
{
public:
    bool isImport;
    bool isSimulation;

    Spdr::CopyMode copyMode;
    Spdr::UpdateMode updateMode;
    Spdr::LogLevel logLevel;
    SpdrSynchronize::SynchronizationOptions synchronizationOptions;

    QString logFile;
    QString inputPath;
    QString outputPath;
};

#endif // SPDRCLIOPTIONS_H

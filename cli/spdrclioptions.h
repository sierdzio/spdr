#ifndef SPDRCLIOPTIONS_H
#define SPDRCLIOPTIONS_H

#include "SpdrBase"
#include "SpdrImport"
#include "SpdrSynchronize"

#include <QString>

/*!
  \ingroup cli

  A container class: holds all the flags and switches that the user has set on
  the command line.
  */
class SpdrCliOptions
{
public:
    bool isImport;
    bool isSimulation;
    bool isFileSuffixCaseSensitive;

    Spdr::UpdateMode updateMode;
    Spdr::LogLevel logLevel;
    SpdrImport::CopyMode copyMode;
    SpdrSynchronize::SynchronizationOptions synchronizationOptions;

    QString logFile;
    QString inputPath;
    QString outputPath;
};

#endif // SPDRCLIOPTIONS_H

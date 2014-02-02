#ifndef SPDRCLIPARSER_H
#define SPDRCLIPARSER_H

#include <QString>
#include <QObject>

#include "SpdrBase"
#include "SpdrSynchronize"

class SpdrCliParser : public QObject
{
    Q_OBJECT
public:
    explicit SpdrCliParser(QObject *parent = 0);
    bool parse();

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

#endif // SPDRCLIPARSER_H

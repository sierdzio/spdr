#ifndef SPDRGUITAGS_H
#define SPDRGUITAGS_H

#include <QString>

struct Tags {
    static QString const windowGeometry;
    static QString const language;

    static QString const inputPathImport;
    static QString const outputPathImport;
    static QString const logPathImport;
    static QString const logLevelImport;
    static QString const updateModeImport;
    static QString const simulateImport;
    static QString const copyModeImport;

    static QString const inputPathSynchronize;
    static QString const outputPathSynchronize;
    static QString const logPathSynchronize;
    static QString const logLevelSynchronize;
    static QString const updateModeSynchronize;
    static QString const simulateSynchronize;
    static QString const removeEmptyDirsSynchronize;
    static QString const removeMissingFilesSynchronize;
    static QString const deepSearchSynchronize;
};

QString const Tags::windowGeometry = "windowGeometry";
QString const Tags::language = "language";

QString const Tags::inputPathImport = "inputPathImport";
QString const Tags::outputPathImport = "outputPathImport";
QString const Tags::logPathImport = "logPathImport";
QString const Tags::logLevelImport = "logLevelImport";
QString const Tags::updateModeImport = "updateModeImport";
QString const Tags::simulateImport = "simulateImport";
QString const Tags::copyModeImport = "copyModeImport";

QString const Tags::inputPathSynchronize = "inputPathSynchronize";
QString const Tags::outputPathSynchronize = "outputPathSynchronize";
QString const Tags::logPathSynchronize = "logPathSynchronize";
QString const Tags::logLevelSynchronize = "logLevelSynchronize";
QString const Tags::updateModeSynchronize = "updateModeSynchronize";
QString const Tags::simulateSynchronize = "simulateSynchronize";
QString const Tags::removeEmptyDirsSynchronize = "removeEmptyDirsSynchronize";
QString const Tags::removeMissingFilesSynchronize = "removeMissingFilesSynchronize";
QString const Tags::deepSearchSynchronize = "deepSearchSynchronize";

#endif // SPDRGUITAGS_H

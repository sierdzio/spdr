#ifndef SPDRGUITAGS_H
#define SPDRGUITAGS_H

#include <QString>

/*!
  \ingroup wigetgui

  \brief Struct containing tags used to explicitly describe, in a shortened form,
  various application functions.

  This is used to read and write Spdr GUI settings.
  */
struct Tags {
    static QString const windowGeometry;
    static QString const language;

    static QString const inputPathImport;
    static QString const outputPathImport;
    static QString const logPathImport;
    static QString const logLevelImport;
    static QString const updateModeImport;
    static QString const simulateImport;
    static QString const suffixCaseSensitivityImport;
    static QString const copyModeImport;

    static QString const inputPathSynchronize;
    static QString const outputPathSynchronize;
    static QString const logPathSynchronize;
    static QString const logLevelSynchronize;
    static QString const updateModeSynchronize;
    static QString const simulateSynchronize;
    static QString const suffixCaseSensitivitySynchronize;
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
QString const Tags::suffixCaseSensitivityImport = "suffixCaseSensitivityImport";
QString const Tags::copyModeImport = "copyModeImport";

QString const Tags::inputPathSynchronize = "inputPathSynchronize";
QString const Tags::outputPathSynchronize = "outputPathSynchronize";
QString const Tags::logPathSynchronize = "logPathSynchronize";
QString const Tags::logLevelSynchronize = "logLevelSynchronize";
QString const Tags::updateModeSynchronize = "updateModeSynchronize";
QString const Tags::simulateSynchronize = "simulateSynchronize";
QString const Tags::suffixCaseSensitivitySynchronize = "suffixCaseSensitivitySynchronize";
QString const Tags::removeEmptyDirsSynchronize = "removeEmptyDirsSynchronize";
QString const Tags::removeMissingFilesSynchronize = "removeMissingFilesSynchronize";
QString const Tags::deepSearchSynchronize = "deepSearchSynchronize";

#endif // SPDRGUITAGS_H

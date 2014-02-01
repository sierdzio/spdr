#ifndef SPDRBASE_H
#define SPDRBASE_H

#include "spdrglobal.h"

#include <QString>
#include <QObject>

class SpdrBasePrivate;

/*!
  Base class for concrete file operation methods implementations available in Spdr.

  It contains common properties and flags that other classes need: input and output
  paths, log functionality, copy settings, etc.
 */
class SPDR_DLLSPEC SpdrBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString inputPath READ inputPath WRITE setInputPath NOTIFY inputPathChanged)
    Q_PROPERTY(QString outputPath READ outputPath WRITE setOutputPath NOTIFY outputPathChanged)
    Q_PROPERTY(bool simulate READ simulate WRITE setSimulate NOTIFY simulateChanged)

    //Q_PROPERTY(Spdr::CopyMode copyMode READ copyMode WRITE setCopyMode NOTIFY copyModeChanged)
    //Q_PROPERTY(Spdr::UpdateMode updateMode READ updateMode WRITE setUpdateMode NOTIFY updateModeChanged)

public:
    explicit SpdrBase(QObject *parent = 0);

    Q_INVOKABLE QString inputPath() const;
    Q_INVOKABLE void setInputPath(const QString &newInputPath);

    Q_INVOKABLE QString outputPath() const;
    Q_INVOKABLE virtual bool setOutputPath(const QString &newOutputPath);

    Q_INVOKABLE bool simulate() const;
    Q_INVOKABLE void setSimulate(bool simulationEnabled);

    Q_INVOKABLE Spdr::CopyMode copyMode() const;
    Q_INVOKABLE void setCopyMode(Spdr::CopyMode newCopyMode);

    Q_INVOKABLE Spdr::UpdateMode updateMode() const;
    Q_INVOKABLE void setUpdateMode(Spdr::UpdateMode newUpdateMode);

    Q_INVOKABLE Spdr::LogLevel logLevel() const;
    Q_INVOKABLE void setLogLevel(Spdr::LogLevel newLevel);

    Q_INVOKABLE bool isUsingLogFile() const;
    Q_INVOKABLE void setLogFile(const QString &logFilePath);
    Q_INVOKABLE QString logFile() const;

public slots:
    void log(const QString &message, Spdr::LogLevel logLevelToUse = Spdr::LogEverything) const;

signals:
    void inputPathChanged(const QString &newInputPath);
    void outputPathChanged(const QString &newOutputPath);
    void simulateChanged(bool simulationEnabled);
    void copyModeChanged(Spdr::CopyMode newCopyMode);
    void updateModeChanged(Spdr::UpdateMode newUpdateMode);

protected:
    SpdrBase(SpdrBasePrivate &dd, QObject *parent = 0);
    SpdrBasePrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(SpdrBase)
};

#endif // SPDRBASE_H

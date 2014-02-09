#ifndef SPDRGUIMAINWINDOW_H
#define SPDRGUIMAINWINDOW_H

#include "spdrguibasicinputform.h"
#include "spdrglobal.h"

#include <QString>
#include <QVariant>
#include <QThread>
#include <QMainWindow>

namespace Ui {
class SpdrGuiMainWindow;
}

class SpdrGuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpdrGuiMainWindow(QWidget *parent = 0);
    ~SpdrGuiMainWindow();

private slots:
    void on_pushButtonSynchronize_clicked();
    void on_pushButtonImport_clicked();
    void on_actionAboutSpdr_triggered();
    void on_actionAboutQt_triggered();
    void on_actionResetSettings_triggered();
    void on_actionQuit_triggered();

    void synchronizationFinished(bool result);
    void importFinished(bool result);
    void messageLogImport(const QString &message, Spdr::LogLevel logLevel);
    void messageLogSynchronize(const QString &message, Spdr::LogLevel logLevel);

private:
    void readSettings();
    void saveSettings();
    void saveSettingValue(const QString &tag, const QVariant &value);

    Ui::SpdrGuiMainWindow *ui;
    SpdrGuiBasicInputForm *importForm;
    SpdrGuiBasicInputForm *synchronizeForm;

    QThread importThread;
    QThread synchronizeThread;

    QString settingsPath;
};

#endif // SPDRGUIMAINWINDOW_H

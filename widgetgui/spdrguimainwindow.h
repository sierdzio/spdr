#ifndef SPDRGUIMAINWINDOW_H
#define SPDRGUIMAINWINDOW_H

#include "spdrguibasicinputform.h"
#include "spdrglobal.h"

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

    void synchronizationFinished(bool result);
    void importFinished(bool result);
    void messageLogImport(const QString &message, Spdr::LogLevel logLevel);
    void messageLogSynchronize(const QString &message, Spdr::LogLevel logLevel);

private:
    Ui::SpdrGuiMainWindow *ui;
    SpdrGuiBasicInputForm *importForm;
    SpdrGuiBasicInputForm *synchronizeForm;

    QThread importThread;
    QThread synchronizeThread;
};

#endif // SPDRGUIMAINWINDOW_H

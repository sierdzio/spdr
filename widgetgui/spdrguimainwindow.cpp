#include "spdrguimainwindow.h"
#include "ui_spdrguimainwindow.h"
#include "ui_spdrguibasicinputform.h"
#include "spdrguitags.h"

#include "SpdrImport"
#include "SpdrSynchronize"

#include <QFile>
#include <QSettings>

#include <QApplication>
#include <QMessageBox>

/*!
  \class SpdrGuiMainWindow
  \ingroup wigetgui

  Class used to display the main user interface.
 */

SpdrGuiMainWindow::SpdrGuiMainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::SpdrGuiMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    importForm = new SpdrGuiBasicInputForm(this);
    synchronizeForm = new SpdrGuiBasicInputForm(this);

    ui->gridLayoutImport->addWidget(importForm, 0, 0);
    ui->gridLayoutSynchronize->addWidget(synchronizeForm, 0, 0);

    settingsPath = qApp->applicationDirPath() + "/spdr-gui-settings.ini";
    readSettings();
}

SpdrGuiMainWindow::~SpdrGuiMainWindow()
{
    importThread.quit();
    importThread.wait();

    synchronizeThread.quit();
    synchronizeThread.wait();

    saveSettings();
    delete ui;
}

void SpdrGuiMainWindow::on_pushButtonSynchronize_clicked()
{
    ui->pushButtonSynchronize->setEnabled(false);
    ui->labelSynchronizeResult->setText(QString::null);

    SpdrSynchronize *synchronizer = new SpdrSynchronize;
    synchronizer->setInputPath(synchronizeForm->lineEditInput->text());
    synchronizer->setOutputPath(synchronizeForm->lineEditOutput->text());
    synchronizer->setLogLevel((Spdr::LogLevel) synchronizeForm->ui->comboBoxLogLevel->currentIndex());
    synchronizer->setUpdateMode((Spdr::UpdateMode) synchronizeForm->ui->comboBoxUpdateMode->currentIndex());
    synchronizer->setSimulate(synchronizeForm->ui->checkBoxSimulate->isChecked());

    if (synchronizeForm->ui->checkBoxLog->isChecked()) {
        synchronizer->setLogFile(synchronizeForm->lineEditLog->text());
    }

    SpdrSynchronize::SynchronizationOptions options;

    if (ui->checkBoxDeepSearch->isChecked()) {
        options |= SpdrSynchronize::DeepSearch;
    }

    if (ui->checkBoxRemoveEmptyDirectiories->isChecked()) {
        options |= SpdrSynchronize::RemoveEmptyDirectories;
    }

    if (ui->checkBoxRemoveMissingFiles->isChecked()) {
        options |= SpdrSynchronize::RemoveMissingFiles;
    }

    synchronizer->setOptions(options);
    synchronizer->moveToThread(&synchronizeThread);

    connect(&synchronizeThread, &QThread::finished, synchronizer, &SpdrSynchronize::deleteLater);
    connect(&synchronizeThread, &QThread::started, synchronizer, &SpdrSynchronize::synchronize);
    connect(synchronizer, &SpdrSynchronize::finished, this,
            &SpdrGuiMainWindow::synchronizationFinished, Qt::QueuedConnection);
    connect(synchronizer, &SpdrSynchronize::logMessage, this,
            &SpdrGuiMainWindow::messageLogSynchronize, Qt::QueuedConnection);

    synchronizeThread.start();
}

void SpdrGuiMainWindow::on_pushButtonImport_clicked()
{
    ui->pushButtonImport->setEnabled(false);
    ui->labelImportResult->setText(QString::null);

    SpdrImport *importer = new SpdrImport;
    importer->setInputPath(importForm->lineEditInput->text());
    importer->setOutputPath(importForm->lineEditOutput->text());
    importer->setLogLevel((Spdr::LogLevel) importForm->ui->comboBoxLogLevel->currentIndex());
    importer->setUpdateMode((Spdr::UpdateMode) importForm->ui->comboBoxUpdateMode->currentIndex());
    importer->setSimulate(importForm->ui->checkBoxSimulate->isChecked());
    importer->setCopyMode((SpdrImport::CopyMode) ui->comboBoxCopyMode->currentIndex());

    if (importForm->ui->checkBoxLog->isChecked()) {
        importer->setLogFile(importForm->lineEditLog->text());
    }

    importer->moveToThread(&importThread);

    connect(&importThread, &QThread::finished, importer, &SpdrImport::deleteLater);
    connect(&importThread, &QThread::started, importer, &SpdrImport::import);
    connect(importer, &SpdrImport::finished,
            this, &SpdrGuiMainWindow::importFinished, Qt::QueuedConnection);
    connect(importer, &SpdrImport::logMessage,
            this, &SpdrGuiMainWindow::messageLogImport, Qt::QueuedConnection);

    importThread.start();
}

void SpdrGuiMainWindow::synchronizationFinished(bool result)
{
    if (result) {
        ui->labelSynchronizeResult->setText(tr("Success"));
    } else {
        ui->labelSynchronizeResult->setText(tr("Error: see the log"));
    }

    synchronizeThread.quit();
    synchronizeThread.wait();

    ui->pushButtonSynchronize->setEnabled(true);
}

void SpdrGuiMainWindow::importFinished(bool result)
{
    if (result) {
        ui->labelImportResult->setText(tr("Success"));
    } else {
        ui->labelImportResult->setText(tr("Error: see the log"));
    }

    importThread.quit();
    importThread.wait();

    ui->pushButtonImport->setEnabled(true);
}

void SpdrGuiMainWindow::messageLogImport(const QString &message, Spdr::LogLevel logLevel)
{
    Q_UNUSED(logLevel);
    ui->labelImportResult->setText(message);
}

void SpdrGuiMainWindow::messageLogSynchronize(const QString &message, Spdr::LogLevel logLevel)
{
    Q_UNUSED(logLevel);
    ui->labelSynchronizeResult->setText(message);
}

void SpdrGuiMainWindow::readSettings()
{
    {
        QFile settingsFile(settingsPath);

        if (!settingsFile.exists()) {
            return;
        }
    }

    QSettings settings(settingsPath, QSettings::IniFormat, this);
    restoreGeometry(settings.value(Tags::windowGeometry, geometry()).toByteArray());

    importForm->lineEditInput->setText(settings.value(Tags::inputPathImport).toString());
    importForm->lineEditOutput->setText(settings.value(Tags::outputPathImport).toString());
    importForm->lineEditLog->setText(settings.value(Tags::logPathImport).toString());
    importForm->ui->comboBoxLogLevel->setCurrentIndex(settings.value(Tags::logLevelImport).toInt());
    importForm->ui->comboBoxUpdateMode->setCurrentIndex(settings.value(Tags::updateModeImport).toInt());
    importForm->ui->checkBoxSimulate->setChecked(settings.value(Tags::simulateImport).toBool());
    ui->comboBoxCopyMode->setCurrentIndex(settings.value(Tags::copyModeImport).toInt());
    importForm->ui->checkBoxLog->setChecked(!importForm->lineEditLog->text().isEmpty());

    synchronizeForm->lineEditInput->setText(settings.value(Tags::inputPathSynchronize).toString());
    synchronizeForm->lineEditOutput->setText(settings.value(Tags::outputPathSynchronize).toString());
    synchronizeForm->lineEditLog->setText(settings.value(Tags::logPathSynchronize).toString());
    synchronizeForm->ui->comboBoxLogLevel->setCurrentIndex(settings.value(Tags::logLevelSynchronize).toInt());
    synchronizeForm->ui->comboBoxUpdateMode->setCurrentIndex(settings.value(Tags::updateModeSynchronize).toInt());
    synchronizeForm->ui->checkBoxSimulate->setChecked(settings.value(Tags::simulateSynchronize).toBool());
    ui->checkBoxRemoveEmptyDirectiories->setChecked(settings.value(Tags::removeEmptyDirsSynchronize).toBool());
    ui->checkBoxRemoveMissingFiles->setChecked(settings.value(Tags::removeMissingFilesSynchronize).toBool());
    ui->checkBoxDeepSearch->setChecked(settings.value(Tags::deepSearchSynchronize).toBool());
    synchronizeForm->ui->checkBoxLog->setChecked(!synchronizeForm->lineEditLog->text().isEmpty());
}

void SpdrGuiMainWindow::saveSettings()
{
    saveSettingValue(Tags::windowGeometry, geometry());

    saveSettingValue(Tags::inputPathImport, importForm->lineEditInput->text());
    saveSettingValue(Tags::outputPathImport, importForm->lineEditOutput->text());
    saveSettingValue(Tags::logPathImport, importForm->lineEditLog->text());
    saveSettingValue(Tags::logLevelImport, importForm->ui->comboBoxLogLevel->currentIndex());
    saveSettingValue(Tags::updateModeImport, importForm->ui->comboBoxUpdateMode->currentIndex());
    saveSettingValue(Tags::simulateImport, importForm->ui->checkBoxSimulate->isChecked());
    saveSettingValue(Tags::copyModeImport, ui->comboBoxCopyMode->currentIndex());

    saveSettingValue(Tags::inputPathSynchronize, synchronizeForm->lineEditInput->text());
    saveSettingValue(Tags::outputPathSynchronize, synchronizeForm->lineEditOutput->text());
    saveSettingValue(Tags::logPathSynchronize, synchronizeForm->lineEditLog->text());
    saveSettingValue(Tags::logLevelSynchronize, synchronizeForm->ui->comboBoxLogLevel->currentIndex());
    saveSettingValue(Tags::updateModeSynchronize, synchronizeForm->ui->comboBoxUpdateMode->currentIndex());
    saveSettingValue(Tags::simulateSynchronize, synchronizeForm->ui->checkBoxSimulate->isChecked());
    saveSettingValue(Tags::removeEmptyDirsSynchronize, ui->checkBoxRemoveEmptyDirectiories->isChecked());
    saveSettingValue(Tags::removeMissingFilesSynchronize, ui->checkBoxRemoveMissingFiles->isChecked());
    saveSettingValue(Tags::deepSearchSynchronize, ui->checkBoxDeepSearch->isChecked());
}

void SpdrGuiMainWindow::saveSettingValue(const QString &tag, const QVariant &value)
{
    QSettings settings(settingsPath, QSettings::IniFormat, this);
    settings.setValue(tag, value);
}

void SpdrGuiMainWindow::on_actionAboutSpdr_triggered()
{
    QString aboutText(tr("Spdr is a file hierarchy management toolkit"));

    QMessageBox::about(this, tr("About Spdr"), aboutText);
}

void SpdrGuiMainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt framework being used in Spdr"));
}

void SpdrGuiMainWindow::on_actionResetSettings_triggered()
{
    QFile settingsFile(settingsPath);

    if (settingsFile.exists()) {
        settingsFile.remove();
    }
}

void SpdrGuiMainWindow::on_actionQuit_triggered()
{
    deleteLater();
    qApp->quit();
}

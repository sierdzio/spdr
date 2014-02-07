#include "spdrguimainwindow.h"
#include "ui_spdrguimainwindow.h"
#include "ui_spdrguibasicinputform.h"

#include "SpdrImport"
#include "SpdrSynchronize"

#include <QApplication>

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
}

SpdrGuiMainWindow::~SpdrGuiMainWindow()
{
    importThread.quit();
    importThread.wait();

    synchronizeThread.quit();
    synchronizeThread.wait();

    delete ui;
}

void SpdrGuiMainWindow::on_pushButtonSynchronize_clicked()
{
    ui->pushButtonSynchronize->setEnabled(false);

    SpdrSynchronize *synchronizer = new SpdrSynchronize;
    synchronizer->setInputPath(synchronizeForm->ui->lineEditInput->text());
    synchronizer->setOutputPath(synchronizeForm->ui->lineEditOutput->text());
    synchronizer->setLogLevel((Spdr::LogLevel) synchronizeForm->ui->comboBoxSynchronizeLogLevel->currentIndex());
    synchronizer->setSimulate(synchronizeForm->ui->checkBoxSimulate->isChecked());

    if (synchronizeForm->ui->checkBoxLog->isChecked()) {
        synchronizer->setLogFile(synchronizeForm->ui->lineEditLog->text());
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
    connect(synchronizer, &SpdrSynchronize::finished, this, &SpdrGuiMainWindow::synchronizationFinished, Qt::QueuedConnection);

    synchronizeThread.start();
}

void SpdrGuiMainWindow::on_pushButtonImport_clicked()
{
    ui->pushButtonImport->setEnabled(false);

    SpdrImport *importer = new SpdrImport;
    importer->setInputPath(importForm->ui->lineEditInput->text());
    importer->setOutputPath(importForm->ui->lineEditOutput->text());
    importer->setLogLevel((Spdr::LogLevel) importForm->ui->comboBoxSynchronizeLogLevel->currentIndex());
    importer->setSimulate(importForm->ui->checkBoxSimulate->isChecked());
    importer->setCopyMode((SpdrImport::CopyMode) ui->comboBoxCopyMode->currentIndex());

    if (importForm->ui->checkBoxLog->isChecked()) {
        importer->setLogFile(importForm->ui->lineEditLog->text());
    }

    importer->moveToThread(&importThread);

    connect(&importThread, &QThread::finished, importer, &SpdrImport::deleteLater);
    connect(&importThread, &QThread::started, importer, &SpdrImport::import);
    connect(importer, &SpdrImport::finished, this, &SpdrGuiMainWindow::importFinished, Qt::QueuedConnection);

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

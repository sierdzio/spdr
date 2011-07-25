#include "../headers/pdmainwindow.h"
#include "ui_pdmainwindow.h"

PDMainWindow::PDMainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::PDMainWindow)
{
    appVersion = "beta1.1";

    ui->setupUi(this);
    centralWidget()->setLayout(ui->mainLayout);

    // Validation:
//    QRegExp formatRegExp("([a-z0-9]{3,4}\\s)*", Qt::CaseInsensitive);
//    ui->lineEditDCustomFormat->setValidator(new QRegExpValidator(formatRegExp, this));


    ui->tabDownloader->setLayout(ui->downloaderLayout);
    ui->tabRedistributor->setLayout(ui->redistributorLayout);
    ui->progressBar->setValue(0);
    ui->progressBarRedistributor->setValue(0);
    ui->tabWidget->setCurrentWidget(ui->tabDownloader);
    ui->actionExit->setShortcut(QKeySequence::Close);

    setWindowTitle(tr("sPDar - %1").arg(appVersion));

    loadSettings();  
}

PDMainWindow::~PDMainWindow()
{
    saveSettings();
    delete ui;
}

void PDMainWindow::loadSettings()
{
    QSettings settings("sierdzio", "sPDaR");

    restoreGeometry(settings.value("geometry").toByteArray());
    ui->lineEditExport->setText(settings.value("exportPath", "").toString());
    ui->lineEditImport->setText(settings.value("importPath", "").toString());
    ui->lineEditTo->setText(settings.value("toPath", "").toString());
    ui->lineEditFrom->setText(settings.value("fromPath", "").toString());
    preferences = settings.value("preferences").toMap();

    initFormatsListsFromSettings();
}

void PDMainWindow::saveSettings()
{
    QSettings settings("sierdzio", "sPDaR");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("exportPath", ui->lineEditExport->text());
    settings.setValue("importPath", ui->lineEditImport->text());
    settings.setValue("toPath", ui->lineEditTo->text());
    settings.setValue("fromPath", ui->lineEditFrom->text());
    settings.setValue("preferences", preferences);
}

void PDMainWindow::on_actionExit_triggered()
{
    QWidget::close();
}

void PDMainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr("About sPDaR"),
                       tr("sierdzio's photoDownloader and redistributor  - %1<br />"
                          "distributed under GNU LGPL 2.1 "
                          "with Qt exception. "
                          "Whole code is available on "
                          "<a href=\"https://sourceforge.net/projects/spdar/\">SourceForge</a>"
                          "<br />Author: Tomasz Siekierda<br />"
                          "<br /><a href=\"http://www.sierdzio.com\">http://www.sierdzio.com</a>"
                          "<br /><a href=\"sierdzio@gmail.com\">sierdzio@gmail.com</a>"
                          "<br /><br />"
                          "Icons used in this program were made by Daniel Fore, and are available here: "
                          "<a href=\"http://danrabbit.deviantart.com/art/elementary-Icons-65437279\">Elementary icons</a>"
                          "<br /><br />"
                          "Running on: Qt%2 libraries."
                          "<br /><br />"
                          "The program is provided AS IS with NO WARRANTY OF ANY KIND, "
                          "INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A "
                          "PARTICULAR PURPOSE."
                          ).arg(appVersion).arg(qVersion()));
}

void PDMainWindow::on_pushButtonImport_clicked()
{
    QString ImportDirPath = QFileDialog::getExistingDirectory(this,
                                              tr("Type the folder to copy from (for now - 102CANON)",
                                                 "/media/"));
    ui->lineEditImport->setText(ImportDirPath);
}

void PDMainWindow::on_pushButtonExport_clicked()
{
    QString ExportDirPath = QFileDialog::getExistingDirectory(this,
                                                              tr("Output folder"),
                                                              "/media/archiwum/");
    ui->lineEditExport->setText(ExportDirPath);
}

void PDMainWindow::on_pushButtonTransfer_clicked()
{
    ui->progressBar->setValue(0);

    //We block user interference in the process of copying:
    ui->pushButtonTransfer->setEnabled(FALSE);

    //We get paths of import and export folders:
    PDDownloader *downloader;
    downloader = new PDDownloader(this, ui->lineEditImport->text(), ui->lineEditExport->text());
    downloader->setFormats(dFormatFilst);
    connect(downloader, SIGNAL(updateProgressBar(int)), this, SLOT(updateDownloaderProgressBar(int)));
    downloader->download(1);

    delete downloader;

    //We relaunch previously disabled elements:
    ui->pushButtonTransfer->setEnabled(TRUE);
}

void PDMainWindow::on_pushButtonFrom_clicked()
{
    QString FromDirPath = QFileDialog::getExistingDirectory(this, tr("Redistribute from where: "
                                                                     "(year):"), "/media/");
    ui->lineEditFrom->setText(FromDirPath);
}

void PDMainWindow::on_pushButtonTo_clicked()
{
    QString ToDirPath = QFileDialog::getExistingDirectory(this, tr("Destination folder:"), "/media/");
    ui->lineEditTo->setText(ToDirPath);
}

void PDMainWindow::on_pushButtonTransferBack_clicked()
{
    ui->progressBarRedistributor->setValue(0);

    //Time to disable the controls for the duration of redistributing:
    ui->pushButtonTransferBack->setEnabled(FALSE);

    PDRedistributor *redistributor;
    redistributor = new PDRedistributor(this, ui->lineEditFrom->text(), ui->lineEditTo->text());
    redistributor->setFormats(rFormatList);
    connect(redistributor, SIGNAL(updateProgressBar(int)), this, SLOT(updateRedistributorProgressBar(int)));
    redistributor->redistribute();

    delete redistributor;

    //After success, we reenable the ui:
    ui->pushButtonTransferBack->setEnabled(TRUE);
}

void PDMainWindow::updateDownloaderProgressBar(int val)
{
    ui->progressBar->setValue(val);
}

void PDMainWindow::updateRedistributorProgressBar(int val)
{
    ui->progressBarRedistributor->setValue(val);
}

void PDMainWindow::on_actionPreferences_triggered()
{
    settingsDialog = new pdSettingsDialog(preferences, this);
    connect(settingsDialog, SIGNAL(accepted()), this, SLOT(on_settingsDialog_accepted()));
    settingsDialog->exec();
}

void PDMainWindow::on_settingsDialog_accepted()
{
    preferences = settingsDialog->preferences();
    dFormatFilst = settingsDialog->downloadFormatsList();
    rFormatList = settingsDialog->redistributeFormatsList();

    delete settingsDialog;
}

void PDMainWindow::initFormatsListsFromSettings()
{
    QStringList tempList;

    // Set download preferences:
    tempList = preferences.value("dFormats", "").toStringList();
    if (preferences.value("dBmp", FALSE).toBool())
        tempList.append("*.bmp");
    if (preferences.value("dCanon", FALSE).toBool())
        tempList.append("*.cr2");
    if (preferences.value("dDng", FALSE).toBool())
        tempList.append("*.dng");
    if (preferences.value("dJpg", FALSE).toBool())
    {
        tempList.append("*.jpg");
        tempList.append("*.jpeg");
    }
    if (preferences.value("dNikon", FALSE).toBool())
        tempList.append("*.nef");
    if (preferences.value("dOlympus", FALSE).toBool())
        tempList.append("*.orf");
    if (preferences.value("dPentax", FALSE).toBool())
        tempList.append("*.pef");
    if (preferences.value("dPng", FALSE).toBool())
        tempList.append("*.png");
    if (preferences.value("dSigma", FALSE).toBool())
        tempList.append("*.x3f");
    if (preferences.value("dSony", FALSE).toBool())
    {
        tempList.append("*.arw");
        tempList.append("*.sr2");
        tempList.append("*.srf");
    }
    if (preferences.value("dTiff", FALSE).toBool())
    {
        tempList.append("*.tif");
        tempList.append("*.tiff");
    }

    dFormatFilst = tempList;
    tempList.clear();

    // Set redistribution preferences:
    tempList = preferences.value("rFormats", "").toStringList();
    if (preferences.value("rAll", FALSE).toBool())
        if (preferences.value("rBmp", FALSE).toBool())
            tempList.append("*.bmp");
//    if (preferences.value("rCanon", FALSE).toBool())
//        tempList.append("*.cr2");
//    if (preferences.value("rDng", FALSE).toBool())
//        tempList.append("*.dng");
    if (preferences.value("rJpg", FALSE).toBool())
    {
        tempList.append("*.jpg");
        tempList.append("*.jpeg");
    }
//    if (preferences.value("rNikon", FALSE).toBool())
//        tempList.append("*.nef");
//    if (preferencess.value("rOlympus", FALSE).toBool())
//        tempList.append("*.orf");
//    if (preferences.value("rPentax", FALSE).toBool())
//        tempList.append("*.pef");
    if (preferences.value("rPng", FALSE).toBool())
        tempList.append("*.png");
//    if (preferences.value("rSigma", FALSE).toBool())
//        tempList.append("*.x3f");
//    if (preferences.value("rSony", FALSE).toBool())
//    {
//        tempList.append("*.arw");
//        tempList.append("*.sr2");
//        tempList.append("*.srf");
//    }
    if (preferences.value("rTiff", FALSE).toBool())
    {
        tempList.append("*.tif");
        tempList.append("*.tiff");
    }

    rFormatList = tempList;
}

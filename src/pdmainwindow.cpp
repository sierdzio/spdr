#include "pdmainwindow.h"
#include "ui_pdmainwindow.h"
#include "pddownloader.h"
#include "pdredistributor.h"
#include "pdsettingsdialog.h"

#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>

/*!
    \class PDMainWindow
    \brief sPDaR's main window's logic is living here.

    Sets up the UI, manages whole user interaction (save for settings dialog).
  */

/*!
    \fn PDMainWindow::PDMainWindow(QWidget *parent)

    Constructs the UI, resets all the vars, loads application settings (using QSettings).

    \a parent defaults to 0.
  */
PDMainWindow::PDMainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::PDMainWindow)
{
    appVersion = "beta2.1";

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

/*!
    \internal

    PDMainWindow's destructor.
  */
PDMainWindow::~PDMainWindow()
{
    saveSettings();
    delete ui;
}

/*!
    \fn PDMainWindow::loadSettings()

    Loads settings using QSettings. This means that:
    \list
    \o on Windows, app settings from system registry are used
    \o on Linux, app settings from /home/sierdzio/.config/sierdzio/sPDaR.conf are used
    \o on Mac - I don't know how, but it works
    \endlist
  */
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

/*!
    \fn PDMainWindow::saveSettings()

    Saves settings using QSettings. This means that:
    \list
    \o on Windows, app settings are written to system registry
    \o on Linux, app settings are written to /home/sierdzio/.config/sierdzio/sPDaR.conf
    \o on Mac - I don't know how, but it works.
    \endlist
  */
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

/*!
    \fn PDMainWindow::on_actionExit_triggered()

    Closes the application.
    */
void PDMainWindow::on_actionExit_triggered()
{
    QWidget::close();
}

/*!
    \fn PDMainWindow::on_actionAbout_triggered()

    Displays the "About" menu.
    */
void PDMainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       tr("About sPDaR"),
                       tr("sierdzio's photoDownloader and redistributor  - %1.<br />"
                          "Distributed under GNU GPL 3.<br/>"
                          "Whole code is available on "
                          "<a href=\"https://sourceforge.net/projects/spdar/\">SourceForge</a>"
                          " and <a href=\"https://gitorious.org/spdar/\">Gitorious</a>"
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

/*!
    \fn PDMainWindow::on_pushButtonImport_clicked()

    Displays a directory dialog, allowing users to select import dirs.
    */
void PDMainWindow::on_pushButtonImport_clicked()
{
    QString ImportDirPath = QFileDialog::getExistingDirectory(this,
                                              tr("Type the folder to copy from (for now - 102CANON)",
                                                 "/media/"));
    ui->lineEditImport->setText(ImportDirPath);
}

/*!
    \fn PDMainWindow::on_pushButtonExport_clicked()

    Displays a directory dialog, allowing users to select export dirs.
    */
void PDMainWindow::on_pushButtonExport_clicked()
{
    QString ExportDirPath = QFileDialog::getExistingDirectory(this,
                                                              tr("Output folder"),
                                                              "/media/archiwum/");
    ui->lineEditExport->setText(ExportDirPath);
}

/*!
    \fn PDMainWindow::on_pushButtonTransfer_clicked()

    This action fires the transfer. Downloader uses specified import
    and export dirs to properly copy all files matching filters to appropriate folders.
    */
void PDMainWindow::on_pushButtonTransfer_clicked()
{
    ui->progressBar->setValue(0);

    //We block user interference in the process of copying:
    ui->pushButtonTransfer->setEnabled(FALSE);

    //We get paths of import and export folders:
    PDDownloader *downloader = new PDDownloader(ui->lineEditImport->text(), ui->lineEditExport->text(), this);
    downloader->setFormats(dFormatFilst);
    connect(downloader, SIGNAL(updateProgressBar(int)), this, SLOT(updateDownloaderProgressBar(int)));
    downloader->download(1);

    delete downloader;

    //We relaunch previously disabled elements:
    ui->pushButtonTransfer->setEnabled(TRUE);
}

/*!
    \fn PDMainWindow::on_pushButtonFrom_clicked()

    Displays a directory dialog, allowing users to select "from" folder
    in redistributor.
    */
void PDMainWindow::on_pushButtonFrom_clicked()
{
    QString FromDirPath = QFileDialog::getExistingDirectory(this, tr("Redistribute from where: "
                                                                     "(year):"), "/media/");
    ui->lineEditFrom->setText(FromDirPath);
}

/*!
    \fn PDMainWindow::on_pushButtonTo_clicked()

    Displays a directory dialog, allowing users to select "to" folder
    in redistributor.
    */
void PDMainWindow::on_pushButtonTo_clicked()
{
    QString ToDirPath = QFileDialog::getExistingDirectory(this, tr("Destination folder:"), "/media/");
    ui->lineEditTo->setText(ToDirPath);
}

/*!
    \fn PDMainWindow::on_pushButtonTransferBack_clicked()

    This function starts the redistribution process. As with downloader, it uses specified directories and filters to copy/ move the data.

    Redistributor moves (that is copies and deletes from source dir, pastes to destination dir)
    files that have RAW equivalent, and copies all else. This means that after you develop your RAWs,
    they are left in source, and only the resulting files (usually JPGs) are being copied to destination.
    Files that did not have any RAW base file are being copied - so you end up with one copy in source, and another
    in destination.
    */
void PDMainWindow::on_pushButtonTransferBack_clicked()
{
    ui->progressBarRedistributor->setValue(0);

    //Time to disable the controls for the duration of redistributing:
    ui->pushButtonTransferBack->setEnabled(FALSE);

    PDRedistributor *redistributor = new PDRedistributor(ui->lineEditFrom->text(), ui->lineEditTo->text(), this);
    redistributor->setFormats(rFormatList);
    connect(redistributor, SIGNAL(updateProgressBar(int)), this, SLOT(updateRedistributorProgressBar(int)));
    redistributor->redistribute();

    delete redistributor;

    //After success, we reenable the ui:
    ui->pushButtonTransferBack->setEnabled(TRUE);
}

/*!
    \fn PDMainWindow::updateDownloaderProgressBar(int val)

    Updates progress bar.
    */
void PDMainWindow::updateDownloaderProgressBar(int val)
{
    ui->progressBar->setValue(val);
}

/*!
    \fn PDMainWindow::updateRedistributorProgressBar(int val)

    Updates redistributor's progress bar.
    */
void PDMainWindow::updateRedistributorProgressBar(int val)
{
    ui->progressBarRedistributor->setValue(val);
}

/*!
    \fn PDMainWindow::on_actionPreferences_triggered()

    Displays the settings dialog.
    */
void PDMainWindow::on_actionPreferences_triggered()
{
    settingsDialog = new pdSettingsDialog(preferences, this);
    connect(settingsDialog, SIGNAL(accepted()), this, SLOT(on_settingsDialog_accepted()));
    settingsDialog->exec();
}

/*!
    \fn PDMainWindow::on_settingsDialog_accepted()

    Fired after setting dialog is accepted.
    */
void PDMainWindow::on_settingsDialog_accepted()
{
    preferences = settingsDialog->preferences();
    dFormatFilst = settingsDialog->downloadFormatsList();
    rFormatList = settingsDialog->redistributeFormatsList();

    delete settingsDialog;
}

/*!
    \fn PDMainWindow::initFormatsListsFromSettings()

    Initialises format lists (that is, filters).
    */
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

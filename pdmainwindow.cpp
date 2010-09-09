#include "pdmainwindow.h"
#include "ui_pdmainwindow.h"

PDMainWindow::PDMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PDMainWindow)
{
    appVersion = "alpha1";

    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->progressBarRedistributor->setValue(0);
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
    QSettings settings("sierdzio", "photoDownloader");
    if (settings.contains("geometry") == TRUE)
    {
        restoreGeometry(settings.value("geometry").toByteArray());
        ui->lineEditExport->setText(settings.value("exportPath").toString());
        ui->lineEditImport->setText(settings.value("importPath").toString());
        ui->lineEditTo->setText(settings.value("toPath").toString());
        ui->lineEditFrom->setText(settings.value("fromPath").toString());
    }
}

void PDMainWindow::saveSettings()
{
    QSettings settings("sierdzio", "photoDownloader");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("exportPath", ui->lineEditExport->text());
    settings.setValue("importPath", ui->lineEditImport->text());
    settings.setValue("toPath", ui->lineEditTo->text());
    settings.setValue("fromPath", ui->lineEditFrom->text());
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
                          "<br />Author: Tomasz Siekierda"
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

#include "../headers/pddownloader.h"

/*!
    \class PDDownloader
    \brief This is main class responsible for downloading photos from camera/ device.

    PDDownloader does all the heavy lifting when downloading photos.

    All the files matching filter lists (set up in PDSettingsDialog) are copied into
    a specified folder hierarchy (currently it's <year>/<month>/<day>/) using one of
    folder handlers PDFolderHandlers
    */

/*!
    \fn PDDownloader::PDDownloader(QObject *parent, QString importDir, QString exportDir)

    Constructs the downloader using specified import and export directories.
  */
PDDownloader::PDDownloader(QObject *parent, QString importDir, QString exportDir) :
    QObject(parent),
    importFolder(importDir),
    exportFolder(exportDir)
{
    folderHandlers = new PDFolderHandlers(this);
}

PDDownloader::~PDDownloader()
{
    delete folderHandlers;
}

/*!
    \fn PDDownloader::download(quint8 handlerType)

    Performs file fownload.
  */
void PDDownloader::download(quint8 handlerType)
{
    QString tempImport = importFolder;
    QString tempExport = exportFolder;
    QString destination = ".";
    QDateTime tempDate;

    //We set up file filters and establish a list of files present in imported directory:
    QDir importDir;
    importDir.setCurrent(tempImport);
    importDir.setFilter(QDir::Files);
    importDir.setSorting(QDir::Time);

    QStringList extensionFilter = formats;
//    extensionFilter << "*.jpg" << "*.JPG" << "*.CR2" << "*.cr2";

    importDir.setNameFilters(extensionFilter);
    QStringList fileList = importDir.entryList();

    //We get through the list, deciding what to do:
    for (int i = 0; i < fileList.size(); i++)
    {
        //We prepare the file to copy:
        bool sta = TRUE;
        QDir::setCurrent(tempImport);
        QFile currentCopyFile;

        currentCopyFile.setFileName(fileList.at(i).toLocal8Bit().constData());
        sta = currentCopyFile.open(QIODevice::ReadWrite);
        //file sorting:
        QFileInfo curInfo(currentCopyFile);
        tempDate = curInfo.created();

        //Folder handling method should be determined here (based on what user chooses):
        if (handlerType == 1)
            destination = folderHandlers->folderHandlerType1(tempDate, tempExport);

        QDir::setCurrent(tempImport);
        //And copy it, with basic error detection:
        sta = currentCopyFile.copy(destination + fileList.at(i).toLocal8Bit().constData());
        emit updateProgressBar(((i + 1) * 100) / (fileList.size()));

        currentCopyFile.close();
    }
}

/*!
    \fn PDDownloader::setFormats(QStringList newFormats)

    Resets the list of formats.
  */
void PDDownloader::setFormats(QStringList newFormats)
{
    formats = newFormats;
}

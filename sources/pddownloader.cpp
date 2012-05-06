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

    Constructs the downloader using specified import (\a importDir) and export (\a exportDir) directories. Needs a \a parent.
  */
PDDownloader::PDDownloader(QObject *parent, QString importDir, QString exportDir) :
    QObject(parent),
    importFolder(importDir),
    exportFolder(exportDir)
{
    folderHandlers = new PDFolderHandlers(this);
}

/*!
    \internal

    PDDownloader's destructor.
  */
PDDownloader::~PDDownloader()
{
    delete folderHandlers;
}

/*!
  \fn PDDownloader::updateProgressBar(int value)

  Sends the signal with new \a value for progress bar.
  */

/*!
    \fn PDDownloader::download(quint8 handlerType)

    Performs file fownload. Specifying the \a handlerType is compulsory, even though
    currently only '1' is valid.
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

    importDir.setNameFilters(extensionFilter);
    QStringList fileList = importDir.entryList();

    //We get through the list, deciding what to do:
    for (int i = 0; i < fileList.size(); i++)
    {
        //We prepare the file to copy:
        QDir::setCurrent(tempImport);
        QFile currentCopyFile;

        currentCopyFile.setFileName(fileList.at(i).toLocal8Bit().constData());
        if (!currentCopyFile.open(QIODevice::ReadWrite)) {
            // TODO: handle error
        }

        //file sorting:
        QFileInfo curInfo(currentCopyFile);
        tempDate = curInfo.created();

        //Folder handling method should be determined here (based on what user chooses):
        if (handlerType == 1)
            destination = folderHandlers->folderHandlerType1(tempDate, tempExport);

        QDir::setCurrent(tempImport);
        //And copy it, with basic error detection:
        if (!currentCopyFile.copy(destination + fileList.at(i).toLocal8Bit().constData())) {
            // TODO: handle error
        }

        emit updateProgressBar(((i + 1) * 100) / (fileList.size()));

        currentCopyFile.close();
    }
}

/*!
    \fn PDDownloader::setFormats(QStringList newFormats)

    Resets the list of formats, using \a newFormats.
  */
void PDDownloader::setFormats(QStringList newFormats)
{
    formats = newFormats;
}

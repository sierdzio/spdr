#include "../include/pdredistributor.h"

/*!
    \class PDRedistributor
    \brief Main clas for the redistribution part of the application.

    Responsible for redistributing developed photos from RAW source dir.

    Redistributor moves (that is copies and deletes from source dir, pastes to destination dir)
    files that have RAW equivalent, and copies all else. This means that after you develop your RAWs,
    they are left in source, and only the resulting files (usually JPGs) are being copied to destination.
    Files that did not have any RAW base file are being copied - so you end up with one copy in source, and another
    in destination.
    */

/*!
    \fn PDRedistributor::PDRedistributor(QObject *parent, QString fromDir, QString toDir)

    Constructs the redistributor, using from (\a fromDir) and to (\a toDir) directory information. Specifying
    \a parent is compulsory.
    */
PDRedistributor::PDRedistributor(QObject *parent, QString fromDir, QString toDir) :
    QObject(parent),
    fromFolder(fromDir),
    toFolder(toDir)
{
    folderHandlers = new PDFolderHandlers(this);
}

/*!
    \internal

    Destructor.
    */
PDRedistributor::~PDRedistributor()
{
    delete folderHandlers;
}

/*!
  \fn PDRedistributor::updateProgressBar(int value)

  Sends the signal with new \a value for progress bar.
  */

/*!
    \fn PDRedistributor::redistribute()

    Central method of redistributor. Does all the work :)
    */
void PDRedistributor::redistribute()
{
    /* Here, we've go to:
     * 1. Move .JPG's from FromDirPath to ToDirPath
     * 2. Remember to create or check folder structure in destination and source.
     * 3. Make sure there's no .JPG in FromDirPath, and nothing more than .JPG in ToDirPath.
     */

    //We establish iteration variables:
    int l,j,k;

    //We gather needed data:
    QString tempFrom = fromFolder;
    QString tempTo = toFolder;
    QString redDestination = ".";
    QDateTime redTempDate;
    QString tempFromDate;

    //Now we need to find directory lists, and prepare to transfer in general:
    //Note: we need monthList, dateList, and fileList for all dirs
    QDir monthListFromDir;
    monthListFromDir.setCurrent(tempFrom);
    monthListFromDir.setFilter(QDir::Dirs);
    //monthList.setSorting(QDir::Time);
    QStringList monthFilterFrom;
    monthFilterFrom << "??";
    monthListFromDir.setNameFilters(monthFilterFrom);
    QStringList monthListFrom = monthListFromDir.entryList();


    if (monthListFrom.size() > 1)
    {
        for (l = 1; l < monthListFrom.size(); l++)
        {
            tempFromDate = tempFrom + "/" + monthListFrom.at(l);

            QDir dateListFromDir;
            dateListFromDir.setCurrent(tempFromDate);
            dateListFromDir.setFilter(QDir::Dirs);
            QStringList dateFilterFrom;
            dateFilterFrom << "????_??_??";
            dateListFromDir.setNameFilters(dateFilterFrom);
            QStringList dateListFrom = dateListFromDir.entryList();

            for (j = 0; j < dateListFrom.size(); j++)
            {
                tempFromDate = tempFrom + "/" + monthListFrom.at(l) + "/" + dateListFrom.at(j);

                // Files to move handling:
                QDir fileListFromDir;
                fileListFromDir.setCurrent(tempFromDate);
                fileListFromDir.setFilter(QDir::Files);
                fileListFromDir.setSorting(QDir::Time);

                QStringList extFilter = formats;

                fileListFromDir.setNameFilters(extFilter);
                QStringList fileListFrom = fileListFromDir.entryList();

                // Files to check against handling:
                QDir checkListDir;
                checkListDir.setCurrent(tempFromDate);
                checkListDir.setFilter(QDir::Files);
                checkListDir.setSorting(QDir::Time);

                QStringList rawFilter;
                rawFilter << "*.cr2" << ".nef" << "*.orf"  << "*.pef"<< "*.arw"<< "*.x3f"<< "*.dng";

                checkListDir.setNameFilters(rawFilter);
                QStringList checkList = checkListDir.entryList();


                if (!fileListFrom.isEmpty())
                {
                    for(k = 0; k < fileListFrom.size(); k++)
                    {
                        //We prepare the file to copy:
                        QDir::setCurrent(tempFromDate);
                        QFile currentRedFile;

                        redTempDate.setDate(QDate::fromString(dateListFrom.at(j),"yyyy_MM_dd"));
                        currentRedFile.setFileName(fileListFrom.at(k).toLocal8Bit().constData());
                        if (!currentRedFile.open(QIODevice::ReadWrite)) {
                            // TODO: handle error
                        }

                        //Folder handling method should be determined here (based on what user chooses):
                        redDestination = folderHandlers->folderHandlerType1(redTempDate, tempTo);

                        QDir::setCurrent(tempFromDate);

                        bool copied = false;

                        foreach (QString suffix, rawFilter)
                        {
                            QString tempFName = fileListFrom.at(k), tempSuffix = suffix;
                            tempFName.chop(3);
                            tempSuffix.remove(0, 2);
                            tempFName.append(tempSuffix);

                            if (checkList.contains(tempFName, Qt::CaseInsensitive))
                            {
                                if (!currentRedFile.copy(redDestination
                                                          + fileListFrom.at(k).toLocal8Bit().constData())) {
                                    // TODO: handle error
                                }

                                emit updateProgressBar(((k + 1) * 100) / (fileListFrom.size()));

                                currentRedFile.open(QIODevice::ReadWrite);
                                currentRedFile.remove();
                                currentRedFile.close();

                                copied = true;
                                break;
                            }
                        }

                        if (copied == FALSE)
                        {
                            if (!currentRedFile.copy(redDestination
                                                      + fileListFrom.at(k).toLocal8Bit().constData())) {
                                // TODO: handle error
                            }

                            emit updateProgressBar(((k + 1) * 100) / (fileListFrom.size()));

                            currentRedFile.open(QIODevice::ReadWrite);
                            currentRedFile.close();
                        }
                    } //end of "file" for
                } //end of if
            } //end of "date" for
        }
    } //end of "month" for
    else
    {
        QDir dateListFromDir;
        dateListFromDir.setCurrent(tempFromDate);
        dateListFromDir.setFilter(QDir::Dirs);
        //dateList.setSorting(QDir::Time);
        QStringList dateFilterFrom;
        dateFilterFrom << "????_??_??";
        dateListFromDir.setNameFilters(dateFilterFrom);
        QStringList dateListFrom = dateListFromDir.entryList();

        for (j = 0; j < dateListFrom.size(); j++)
        {
            tempFromDate = tempFrom + "/" + dateListFrom.at(j);
            QDir fileListFromDir;
            fileListFromDir.setCurrent(tempFromDate);
            fileListFromDir.setFilter(QDir::Files);
            fileListFromDir.setSorting(QDir::Time);
            QStringList extFilter = formats;
            fileListFromDir.setNameFilters(extFilter);
            QStringList fileListFrom = fileListFromDir.entryList();

            if (!fileListFrom.isEmpty())
            {
                for(k = 0; k < fileListFrom.size(); k++)
                {
                    //We prepare the file to copy:
                    QDir::setCurrent(tempFromDate);
                    QFile currentRedFile;

                    redTempDate.setDate(QDate::fromString(dateListFrom.at(j),"yyyy_MM_dd"));
                    currentRedFile.setFileName(fileListFrom.at(k).toLocal8Bit().constData());
                    if (!currentRedFile.open(QIODevice::ReadWrite)) {
                        // TODO: handle error
                    }

                    //Folder handling method should be determined here (based on what user chooses):
                    redDestination = folderHandlers->folderHandlerType1(redTempDate, tempTo);

                    QDir::setCurrent(tempFromDate);
                    //And copy it, with basic error detection:
                    if (!currentRedFile.copy(redDestination+fileListFrom.at(k).toLocal8Bit().constData())) {
                        // TODO: handle error
                    }

                    emit updateProgressBar(((k+1)*100)/(fileListFrom.size()));

                    currentRedFile.open(QIODevice::ReadWrite);
                    currentRedFile.remove();
                    currentRedFile.close();
                }
            }
        }
    }
}

/*!
    \fn PDRedistributor::setFormats(QStringList newFormats)

    Resets the formats/ filters, using \a newFormats.
    */
void PDRedistributor::setFormats(QStringList newFormats)
{
    formats = newFormats;
}

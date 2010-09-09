#include "pdredistributor.h"

PDRedistributor::PDRedistributor(QObject *parent, QString fromDir, QString toDir) :
    QObject(parent),
    fromFolder(fromDir),
    toFolder(toDir)
{
    folderHandlers = new PDFolderHandlers(this);
}

PDRedistributor::~PDRedistributor()
{
    delete folderHandlers;
}

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


    if (monthListFrom.size() > 1) {
        for (l = 1; l < monthListFrom.size(); l++)
        {
            tempFromDate = tempFrom + "/" + monthListFrom.at(l);

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
                tempFromDate = tempFrom + "/" + monthListFrom.at(l) + "/" + dateListFrom.at(j);

                // Files to move handling:
                QDir fileListFromDir;
                fileListFromDir.setCurrent(tempFromDate);
                fileListFromDir.setFilter(QDir::Files);
                fileListFromDir.setSorting(QDir::Time);

                QStringList extFilter;
                extFilter << "*.jpg" << "*.JPG" << "*.tif" << "*.tiff";

                fileListFromDir.setNameFilters(extFilter);
                QStringList fileListFrom = fileListFromDir.entryList();

                // Files to check against handling:
                QDir checkListDir;
                checkListDir.setCurrent(tempFromDate);
                checkListDir.setFilter(QDir::Files);
                checkListDir.setSorting(QDir::Time);

                QStringList crFilter;
                crFilter << "*.CR2" << "*.cr2";

                checkListDir.setNameFilters(crFilter);
                QStringList checkList = checkListDir.entryList();


                if (!fileListFrom.isEmpty())
                {
                    for(k = 0; k < fileListFrom.size(); k++)
                    {
                        //We prepare the file to copy:
                        bool sta = TRUE;
                        QDir::setCurrent(tempFromDate);
                        QFile currentRedFile;

                        redTempDate.setDate(QDate::fromString(dateListFrom.at(j),"yyyy_MM_dd"));
                        currentRedFile.setFileName(fileListFrom.at(k).toLocal8Bit().constData());
                        sta = currentRedFile.open(QIODevice::ReadWrite);
                        //file sorting:
                        //QFileInfo curInfo(currentRedFile);
                        //tempDate = curInfo.created();

                        //Folder handling method should be determined here (based on what user chooses):
                        redDestination = folderHandlers->folderHandlerType1(redTempDate, tempTo);

                        QDir::setCurrent(tempFromDate);
                        //And copy it, with basic error detection:
                        QString tempFName = fileListFrom.at(k);
                        tempFName.chop(3);
                        tempFName.append("CR2");
                        if (checkList.contains(tempFName, Qt::CaseInsensitive))
                        {
                            sta = currentRedFile.copy(redDestination
                                                      + fileListFrom.at(k).toLocal8Bit().constData());
                            emit updateProgressBar(((k + 1) * 100) / (fileListFrom.size()));

                            currentRedFile.open(QIODevice::ReadWrite);
                            currentRedFile.remove();
                            currentRedFile.close();
                        }
                        else
                        {
                            sta = currentRedFile.copy(redDestination
                                                      + fileListFrom.at(k).toLocal8Bit().constData());
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
            QStringList extFilter;
            extFilter << "*.jpg" << "*.JPG" << "*.tif" << "*.tiff";
            fileListFromDir.setNameFilters(extFilter);
            QStringList fileListFrom = fileListFromDir.entryList();

            if (!fileListFrom.isEmpty())
            {
                for(k = 0; k < fileListFrom.size(); k++)
                {
                    //We prepare the file to copy:
                    bool sta=TRUE;
                    QDir::setCurrent(tempFromDate);
                    QFile currentRedFile;

                    redTempDate.setDate(QDate::fromString(dateListFrom.at(j),"yyyy_MM_dd"));
                    currentRedFile.setFileName(fileListFrom.at(k).toLocal8Bit().constData());
                    sta = currentRedFile.open(QIODevice::ReadWrite);
                    //file sorting:
                    //QFileInfo curInfo(currentRedFile);
                    //tempDate = curInfo.created();

                    //Folder handling method should be determined here (based on what user chooses):
                    redDestination = folderHandlers->folderHandlerType1(redTempDate, tempTo);

                    QDir::setCurrent(tempFromDate);
                    //And copy it, with basic error detection:
                    sta = currentRedFile.copy(redDestination+fileListFrom.at(k).toLocal8Bit().constData());
                    emit updateProgressBar(((k+1)*100)/(fileListFrom.size()));

                    currentRedFile.open(QIODevice::ReadWrite);
                    currentRedFile.remove();
                    currentRedFile.close();
                }
            }
        }
    }
}

#include "../headers/pdfolderhandlers.h"

/*!
  \class PDFolderHandlers
  \brief This class stores all the folder handlers for Downloader.

  The first and default handler follows the <year>/<month>/<day>/ path.
  */

/*!
  \fn PDFolderHandlers::PDFolderHandlers(QObject *parent)

  Default constructor. Actually, it should be deleted and all methods should be made static. \a parent defaults to 0.
  */
PDFolderHandlers::PDFolderHandlers(QObject *parent) :
    QObject(parent)
{
}

/*!
  \fn PDFolderHandlers::folderHandlerType1 (QDateTime tempDate, QString tempExport)

  Safely traverses the dir structure, so that photos are always copied safely. Needs \a tempDate get through
  hierarchy and \a tempExport to get the base folder path.
  */
QString PDFolderHandlers::folderHandlerType1 (QDateTime tempDate, QString tempExport)
{
     QDir tempDir;
     QString tempDestination = ".";
     QDir::setCurrent(tempExport);

     if (tempDir.exists(tempDate.toString("yyyy")))
     { tempDir.cd(tempDate.toString("yyyy"));
         if (tempDir.exists(tempDate.toString("MM")))
         {
             tempDir.cd(tempDate.toString("MM"));
             if (tempDir.exists(tempDate.toString("yyyy_MM_dd")));
             else tempDir.mkdir(tempDate.toString("yyyy_MM_dd"));
         }
         else
         {
             tempDir.mkdir(tempDate.toString("MM"));
             tempDir.cd(tempDate.toString("MM"));
             tempDir.mkdir(tempDate.toString("yyyy_MM_dd"));
         }
     }
     else
     {
         tempDir.mkdir(tempDate.toString("yyyy"));
         tempDir.cd(tempDate.toString("yyyy"));
         tempDir.mkdir(tempDate.toString("MM"));
         tempDir.cd(tempDate.toString("MM"));
         tempDir.mkdir(tempDate.toString("yyyy_MM_dd"));
     }

     tempDestination = tempExport + "/"
                       + tempDate.toString("yyyy") + "/"
                       + tempDate.toString("MM") + "/"
                       + tempDate.toString("yyyy_MM_dd") + "/";

        return tempDestination;
}

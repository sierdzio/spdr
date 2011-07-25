#include "pdfolderhandlers.h"

PDFolderHandlers::PDFolderHandlers(QObject *parent) :
    QObject(parent)
{
}

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

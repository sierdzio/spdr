#ifndef PDDOWNLOADER_H
#define PDDOWNLOADER_H

#include <QObject>
#include <QtCore>

#include "pdfolderhandlers.h"

/*
 This class downloads photos from a device.
 */
class PDDownloader : public QObject
{
    Q_OBJECT
public:
    explicit PDDownloader(QObject *parent, QString importDir, QString exportDir);
    ~PDDownloader();

signals:
    void updateProgressBar(int value);

public slots:
    void download(quint8 handlerType);
    void setFormats(QStringList newFormats);

private:
    QString importFolder;
    QString exportFolder;
    QStringList formats;

    PDFolderHandlers *folderHandlers;
};

#endif // PDDOWNLOADER_H

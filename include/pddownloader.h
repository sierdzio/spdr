#ifndef PDDOWNLOADER_H
#define PDDOWNLOADER_H

#include <QString>
#include <QStringList>
#include <QObject>

class PDFolderHandlers;

/*
 This class downloads photos from a device.
 */
class PDDownloader : public QObject
{
    Q_OBJECT

public:
    explicit PDDownloader(const QString &importDir, const QString &exportDir,
                          QObject *parent = 0);
    ~PDDownloader();

signals:
    void updateProgressBar(int value);

public slots:
    void download(quint8 handlerType);
    void setFormats(const QStringList &newFormats);

private:
    QString importFolder;
    QString exportFolder;
    QStringList formats;

    PDFolderHandlers *folderHandlers;
};

#endif // PDDOWNLOADER_H

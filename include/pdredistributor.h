#ifndef PDREDISTRIBUTOR_H
#define PDREDISTRIBUTOR_H

#include <QString>
#include <QStringList>
#include <QObject>

class PDFolderHandlers;

/*
 This class redistributes developed photos.
 */
class PDRedistributor : public QObject
{
    Q_OBJECT

public:
    explicit PDRedistributor(const QString &fromDir, const QString &toDir,
                             QObject *parent = 0);
    ~PDRedistributor();

signals:
    void updateProgressBar(int value);

public slots:
    void redistribute();
    void setFormats(const QStringList &newFormats);

private:
    QString fromFolder;
    QString toFolder;
    QStringList formats;

    PDFolderHandlers *folderHandlers;
};

#endif // PDREDISTRIBUTOR_H

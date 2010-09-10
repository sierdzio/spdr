/*
 This class redistributes developed photos.
 */

#ifndef PDREDISTRIBUTOR_H
#define PDREDISTRIBUTOR_H

#include <QtCore>
#include <QObject>

#include "pdfolderhandlers.h"

class PDRedistributor : public QObject
{
    Q_OBJECT
public:
    explicit PDRedistributor(QObject *parent, QString fromDir, QString toDir);
    ~PDRedistributor();

signals:
    void updateProgressBar(int value);

public slots:
    void redistribute();
    void setFormats(QStringList newFormats);

private:
    QString fromFolder;
    QString toFolder;
    QStringList formats;

    PDFolderHandlers *folderHandlers;
};

#endif // PDREDISTRIBUTOR_H

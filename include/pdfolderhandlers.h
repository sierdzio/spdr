#ifndef PDFOLDERHANDLERS_H
#define PDFOLDERHANDLERS_H

#include <QString>
#include <QObject>
#include <QDateTime>

class PDFolderHandlers : public QObject
{
    Q_OBJECT

public:
    explicit PDFolderHandlers(QObject *parent = 0);

public slots:
    QString folderHandlerType1(const QDateTime &tempDate, const QString &tempExport);
};

#endif // PDFOLDERHANDLERS_H

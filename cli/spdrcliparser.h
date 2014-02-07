#ifndef SPDRCLIPARSER_H
#define SPDRCLIPARSER_H

#include <QString>
#include <QObject>

#include "spdrclioptions.h"

class SpdrCliParser : public QObject
{
    Q_OBJECT

public:
    explicit SpdrCliParser(QObject *parent = 0);
    bool parse();

    SpdrCliOptions options;
};

#endif // SPDRCLIPARSER_H

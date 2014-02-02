#ifndef SPDRCLIPARSER_H
#define SPDRCLIPARSER_H

#include <QString>
#include <QObject>

#include "spdrclioptions.h"

/*!
  Parses the command line options for Spdr Command line tool.

  All information is put into public members of this class (for convenience).
 */
class SpdrCliParser : public QObject
{
    Q_OBJECT

public:
    explicit SpdrCliParser(QObject *parent = 0);
    bool parse();

    SpdrCliOptions options;
};

#endif // SPDRCLIPARSER_H

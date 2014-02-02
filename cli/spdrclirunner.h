#ifndef SPDRCLIRUNNER_H
#define SPDRCLIRUNNER_H

#include "spdrclioptions.h"

#include <QObject>

/*!
  Class used to run operations as requested by the user on the command line.

  It can be safely used in a thread.
 */
class SpdrCliRunner : public QObject
{
    Q_OBJECT
public:
    explicit SpdrCliRunner(QObject *parent = 0);
    void setOptions(SpdrCliOptions options);

public slots:
    void performActions();

private:
    SpdrCliOptions mOptions;
};

#endif // SPDRCLIRUNNER_H

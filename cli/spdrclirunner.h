#ifndef SPDRCLIRUNNER_H
#define SPDRCLIRUNNER_H

#include "spdrclioptions.h"

#include <QObject>

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

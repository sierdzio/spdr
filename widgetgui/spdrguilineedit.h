#ifndef SPDRGUILINEEDIT_H
#define SPDRGUILINEEDIT_H

#include <QLineEdit>

class QDropEvent;
class QDragEnterEvent;

class SpdrGuiLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SpdrGuiLineEdit(QWidget *parent = 0);
    bool canBeFile;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // SPDRGUILINEEDIT_H

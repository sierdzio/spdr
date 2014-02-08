#include "spdrguilineedit.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDir>

SpdrGuiLineEdit::SpdrGuiLineEdit(QWidget *parent) : QLineEdit(parent)
{
    setAcceptDrops(true);
}

void SpdrGuiLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->acceptProposedAction();
    }
}

void SpdrGuiLineEdit::dropEvent(QDropEvent *event)
{
    QString dropPath(event->mimeData()->text());
    QDir dropDir(dropPath);

    if (dropDir.exists()) {
        setText(dropDir.absoluteFilePath());
        event->acceptProposedAction();
    }

}

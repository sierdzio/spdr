#include "spdrguilineedit.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDir>
#include <QFileInfo>
#include <QUrl>

SpdrGuiLineEdit::SpdrGuiLineEdit(QWidget *parent) : QLineEdit(parent)
{
    setAcceptDrops(true);
    setPlaceholderText(tr("/some/example/path (you can drag & drop here)"));
    canBeFile = false;
}

void SpdrGuiLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain")) {
        event->acceptProposedAction();
    }
}

void SpdrGuiLineEdit::dropEvent(QDropEvent *event)
{
    QString dropPath(QUrl::fromUserInput(event->mimeData()->text()).toDisplayString(QUrl::PreferLocalFile));
    QFileInfo dropDir(dropPath);

    if ((canBeFile && dropDir.isFile()) || (dropDir.exists() && dropDir.isDir())) {
        setText(dropDir.absoluteFilePath());
        event->acceptProposedAction();
    }
}

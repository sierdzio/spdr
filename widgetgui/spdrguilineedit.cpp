#include "spdrguilineedit.h"

#include <QChar>
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
    if (event->mimeData()->hasText() || event->mimeData()->hasUrls()) { //hasFormat("text/plain")) {
        event->acceptProposedAction();
    }
}

void SpdrGuiLineEdit::dropEvent(QDropEvent *event)
{
    QString dropPath(QUrl::fromUserInput(event->mimeData()->text())
                     .toDisplayString(QUrl::PreferLocalFile | QUrl::NormalizePathSegments));

    // Remove leading slash on Windows (probably a Qt bug)
#ifdef Q_OS_WIN
    if (dropPath.at(0) == QChar('/')) {
        dropPath = dropPath.mid(1);
    }
#endif

    QFileInfo dropDir(dropPath);
    if ((canBeFile && dropDir.isFile()) || (dropDir.exists() && dropDir.isDir())) {
        setText(dropDir.absoluteFilePath());
        event->acceptProposedAction();
    }
}

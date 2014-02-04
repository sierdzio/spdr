#ifndef SPDRGUIMAINWINDOW_H
#define SPDRGUIMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class SpdrGuiMainWindow;
}

class SpdrGuiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpdrGuiMainWindow(QWidget *parent = 0);
    ~SpdrGuiMainWindow();

private:
    Ui::SpdrGuiMainWindow *ui;
};

#endif // SPDRGUIMAINWINDOW_H

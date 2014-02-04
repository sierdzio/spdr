#ifndef SPDRGUIMAINWINDOW_H
#define SPDRGUIMAINWINDOW_H

#include "spdrguibasicinputform.h"

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
    SpdrGuiBasicInputForm *importForm;
    SpdrGuiBasicInputForm *synchronizeForm;
};

#endif // SPDRGUIMAINWINDOW_H

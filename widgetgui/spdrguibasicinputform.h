#ifndef SPDRGUIBASICINPUTFORM_H
#define SPDRGUIBASICINPUTFORM_H

#include <QWidget>

namespace Ui {
class SpdrGuiBasicInputForm;
}

class SpdrGuiBasicInputForm : public QWidget
{
    Q_OBJECT

public:
    explicit SpdrGuiBasicInputForm(QWidget *parent = 0);
    ~SpdrGuiBasicInputForm();

//private:
    Ui::SpdrGuiBasicInputForm *ui;
};

#endif // SPDRGUIBASICINPUTFORM_H

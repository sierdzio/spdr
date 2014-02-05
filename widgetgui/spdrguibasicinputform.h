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

private slots:
    void on_pushButtonInput_clicked();
    void on_pushButtonOutput_clicked();
    void on_pushButtonLog_clicked();
};

#endif // SPDRGUIBASICINPUTFORM_H

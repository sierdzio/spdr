#include "spdrguibasicinputform.h"
#include "ui_spdrguibasicinputform.h"

SpdrGuiBasicInputForm::SpdrGuiBasicInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpdrGuiBasicInputForm)
{
    ui->setupUi(this);
}

SpdrGuiBasicInputForm::~SpdrGuiBasicInputForm()
{
    delete ui;
}

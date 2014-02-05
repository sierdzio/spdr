#include "spdrguibasicinputform.h"
#include "ui_spdrguibasicinputform.h"

#include <QFileDialog>

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

// TODO: read path from settings
void SpdrGuiBasicInputForm::on_pushButtonInput_clicked()
{
    ui->lineEditInput->setText(QFileDialog::getExistingDirectory(this, tr("Specify the input folder")));
}

void SpdrGuiBasicInputForm::on_pushButtonOutput_clicked()
{
    ui->lineEditOutput->setText(QFileDialog::getExistingDirectory(this, tr("Specify the output folder")));
}

void SpdrGuiBasicInputForm::on_pushButtonLog_clicked()
{
    ui->lineEditLog->setText(QFileDialog::getSaveFileName(this, tr("Specify the log file")));
}

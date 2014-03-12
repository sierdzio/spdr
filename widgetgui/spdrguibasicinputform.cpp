#include "spdrguibasicinputform.h"
#include "ui_spdrguibasicinputform.h"

#include <QFileDialog>

/*!
  \class SpdrGuiBasicInputForm
  \ingroup wigetgui

  \brief Class used to display options that are common to both import and sychronize
  functionalities.
 */

SpdrGuiBasicInputForm::SpdrGuiBasicInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpdrGuiBasicInputForm)
{
    ui->setupUi(this);

    QWidget *w1 = ui->inputPlaceholder;
    QWidget *w2 = ui->outputPlaceholder;
    QWidget *w3 = ui->logPlaceholder;

    lineEditInput = new SpdrGuiLineEdit(qobject_cast<QWidget *>(w1->parent()));
    lineEditOutput = new SpdrGuiLineEdit(qobject_cast<QWidget *>(w2->parent()));
    lineEditLog = new SpdrGuiLineEdit(qobject_cast<QWidget *>(w3->parent()));

    ui->gridLayoutMain->replaceWidget(w1, lineEditInput);
    ui->gridLayoutMain->replaceWidget(w2, lineEditOutput);
    ui->horizontalLayout->replaceWidget(w3, lineEditLog);

    lineEditLog->canBeFile = true;
    lineEditLog->setEnabled(ui->checkBoxLog->isChecked());
    connect(ui->checkBoxLog, &QCheckBox::toggled, lineEditLog, &SpdrGuiLineEdit::setEnabled);

    delete w1;
    delete w2;
    delete w3;
}

SpdrGuiBasicInputForm::~SpdrGuiBasicInputForm()
{
    delete ui;
}

// TODO: read path from settings
void SpdrGuiBasicInputForm::on_pushButtonInput_clicked()
{
    lineEditInput->setText(QFileDialog::getExistingDirectory(this, tr("Specify the input folder")));
}

void SpdrGuiBasicInputForm::on_pushButtonOutput_clicked()
{
    lineEditOutput->setText(QFileDialog::getExistingDirectory(this, tr("Specify the output folder")));
}

void SpdrGuiBasicInputForm::on_pushButtonLog_clicked()
{
    lineEditLog->setText(QFileDialog::getSaveFileName(this, tr("Specify the log file")));
}

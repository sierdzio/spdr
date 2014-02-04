#include "spdrguimainwindow.h"
#include "ui_spdrguimainwindow.h"

SpdrGuiMainWindow::SpdrGuiMainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::SpdrGuiMainWindow)
{
    ui->setupUi(this);

    importForm = new SpdrGuiBasicInputForm(this);
    synchronizeForm = new SpdrGuiBasicInputForm(this);

    ui->gridLayoutImport->addWidget(importForm, 0, 0);
    ui->gridLayoutSynchronize->addWidget(synchronizeForm, 0, 0);
}

SpdrGuiMainWindow::~SpdrGuiMainWindow()
{
    delete ui;
}

#include "spdrguimainwindow.h"
#include "ui_spdrguimainwindow.h"

SpdrGuiMainWindow::SpdrGuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpdrGuiMainWindow)
{
    ui->setupUi(this);
}

SpdrGuiMainWindow::~SpdrGuiMainWindow()
{
    delete ui;
}

#include "batchdialog.h"
#include "ui_batchdialog.h"

batchDialog::batchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::batchDialog)
{
    ui->setupUi(this);
}

batchDialog::~batchDialog()
{
    delete ui;
}

#include "checkupdatedialog.h"
#include "ui_checkupdatedialog.h"

checkupdateDialog::checkupdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkupdateDialog)
{
    ui->setupUi(this);
}

checkupdateDialog::~checkupdateDialog()
{
    delete ui;
}

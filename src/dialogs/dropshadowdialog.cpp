#include "dropshadowdialog.h"
#include "ui_dropshadowdialog.h"

dropshadowDialog::dropshadowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dropshadowDialog)
{
    ui->setupUi(this);
}

dropshadowDialog::~dropshadowDialog()
{
    delete ui;
}

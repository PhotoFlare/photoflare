#include "imagepropertiesdialog.h"
#include "ui_imagepropertiesdialog.h"

imagePropertiesDialog::imagePropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::imagePropertiesDialog)
{
    ui->setupUi(this);
}

imagePropertiesDialog::~imagePropertiesDialog()
{
    delete ui;
}

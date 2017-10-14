#include "colourmanagerdialog.h"
#include "ui_colourmanagerdialog.h"

colourManagerDialog::colourManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colourManagerDialog)
{
    ui->setupUi(this);
}

colourManagerDialog::~colourManagerDialog()
{
    delete ui;
}

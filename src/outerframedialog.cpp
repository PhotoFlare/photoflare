#include "outerframedialog.h"
#include "ui_outerframedialog.h"

OuterFrameDialog::OuterFrameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuterFrameDialog)
{
    ui->setupUi(this);
}

OuterFrameDialog::~OuterFrameDialog()
{
    delete ui;
}

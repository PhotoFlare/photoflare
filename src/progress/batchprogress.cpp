#include "batchprogress.h"
#include "ui_batchprogress.h"

BatchProgress::BatchProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchProgress)
{
    ui->setupUi(this);
}

BatchProgress::~BatchProgress()
{
    delete ui;
}

void BatchProgress::progress(int value,int total)
{
    ui->progressBar->setValue(int(100 * (float)value/(float)total));
    if(value == total) 
        hide();
}

void BatchProgress::on_hideButton_clicked()
{
    hide();
}

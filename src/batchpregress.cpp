#include "batchpregress.h"
#include "ui_batchpregress.h"

BatchPregress::BatchPregress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchPregress)
{
    ui->setupUi(this);
}

BatchPregress::~BatchPregress()
{
    delete ui;
}

void BatchPregress::progress(int value,int total)
{
    ui->progressBar->setValue(int(100 * (float)value/(float)total));
    if(value == total) {
        hide();
    }
}

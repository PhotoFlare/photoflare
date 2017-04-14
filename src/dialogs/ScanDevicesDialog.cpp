/*

  ScanDevices dialog class.

*/

#include "ScanDevicesDialog.h"
#include "ui_scandevicesdialog.h"
#include "../Settings.h"

ScanDevicesDialog::ScanDevicesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanDevicesDialog),
    mCurrentSourceId(1)
{
    ui->setupUi(this);
}

ScanDevicesDialog::~ScanDevicesDialog()
{
    delete ui;
}

void ScanDevicesDialog::setSourceList(QStringList sourceList)
{
    ui->listWidget->addItems(sourceList);
    ui->listWidget->setCurrentRow(mCurrentSourceId - 1);
}

void ScanDevicesDialog::on_listWidget_currentRowChanged(int currentRow)
{
    mCurrentSourceId = currentRow + 1;
}

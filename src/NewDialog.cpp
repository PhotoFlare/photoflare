/*

  Dialog for creating a new image.

*/


#include "NewDialog.h"
#include "ui_NewDialog.h"
#include <QTabWidget>
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);

    // Disable parts of the dialog that are not implemented yet.
    ui->doubleSpinBox_2->setEnabled(false);
    ui->doubleSpinBox_3->setEnabled(false);
    ui->doubleSpinBox_4->setEnabled(false);
    ui->imageWHcombo->setEnabled(false);
    ui->imageResCombo->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
}

NewDialog::~NewDialog()
{
    delete ui;
}

QSize NewDialog::newImageSize() const
{
    return m_chosenSize;
}

void NewDialog::on_buttonBox_accepted()
{
    m_chosenSize.setWidth(ui->imageWvalue->value());
    m_chosenSize.setHeight(ui->imageHvalue->value());
}

void NewDialog::on_imagePresetCombo_currentIndexChanged(int index)
{
    if (index==0) {
        ui->imageWvalue->setValue(100);
        ui->imageHvalue->setValue(100);
    }
    if (index==1) {
        ui->imageWvalue->setValue(640);
        ui->imageHvalue->setValue(480);
    }
    if (index==2) {
        ui->imageWvalue->setValue(800);
        ui->imageHvalue->setValue(600);
    }
}

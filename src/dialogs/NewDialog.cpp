/*

  Dialog for creating a new image.

*/

//#include <QDebug>
//#include <cmath>
#include <QTabWidget>
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QColorDialog>

#include "NewDialog.h"
#include "ui_NewDialog.h"

#include "Settings.h"
#include "imagepositionwidget.h"

enum {PPM, PPI};
enum {PX, CM, IN};
enum {x0y0, x100y100, x640y480, x800y600, x1024y768, x1366y768, x1600y900, x1920y1080, x3840y2160};

#define CM_IN_INCH 2.54f

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);

    width_px = 1024;
    height_px = 768;
    imageRatio = (width_px/height_px);
    currentUnit = PX;
    ui->imageResCombo->setCurrentIndex(PPI);
    //Standard ppi
    ui->imageRvalue->setValue(96.0);
    ui->pixelWvalue->setValue(width_px);
    ui->pixelHvalue->setValue(height_px);
    ui->memoryValue->setValue((((width_px * height_px) * 3)/1024)/1024);

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->backgroundColorComboBox->width(),ui->backgroundColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->backgroundColorComboBox->addItem(QString(), pixmap);
        ui->backgroundColorComboBox_NewFile->addItem(QString(), pixmap);
    }

    ui->backgroundColorComboBox->setOnClickHandler(this);

    setMode(ResizeImage);
}

NewDialog::~NewDialog()
{
    delete ui;
}

QSize NewDialog::newImageSize() const
{
    return m_chosenSize;
}

QColor NewDialog::newImageBackgroundColor() const
{
    QImage img = (QImage)ui->backgroundColorComboBox_NewFile->currentData().value<QImage>();
    return img.pixel(0,0);
}

void NewDialog::setImageSize(QSize size)
{
    ui->imageWvalue->setValue(size.width());
    ui->imageHvalue->setValue(size.height());
}

QColor NewDialog::backgroundColor() const
{
    QImage img = (QImage)ui->backgroundColorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

void NewDialog::on_buttonBox_accepted()
{
    m_chosenSize.setWidth(width_px);
    m_chosenSize.setHeight(height_px);
}

void NewDialog::on_imagePresetCombo_currentIndexChanged(int index)
{
    if (index == x0y0)
    {
        width_px = 0;
        height_px = 0;
    }
    else if (index == x100y100)
    {
        width_px = 100;
        height_px = 100;
    }
    else if (index == x640y480)
    {
        width_px = 640;
        height_px = 480;
    }
    else if (index == x800y600)
    {
        width_px = 800;
        height_px = 600;
    }
    else if (index == x1024y768)
    {
        width_px = 1024;
        height_px = 768;
    }
    else if (index == x1366y768)
    {
        width_px = 1366;
        height_px = 768;
    }
    else if (index == x1600y900)
    {
        width_px = 1600;
        height_px = 900;
    }
    else if (index == x1920y1080)
    {
        width_px = 1920;
        height_px = 1080;
    }
    else if (index == x3840y2160)
    {
        width_px = 3840;
        height_px = 2160;
    }
    on_imageWHcombo_currentIndexChanged(currentUnit);
}

void NewDialog::on_imageResCombo_currentIndexChanged(int index)
{
    if(index == PPI)
    {
        ui->imageRvalue->setValue(ui->imageRvalue->value() * CM_IN_INCH);
    }
    else
    {
        ui->imageRvalue->setValue(ui->imageRvalue->value() / CM_IN_INCH);
    }
}

void NewDialog::on_imageWHcombo_currentIndexChanged(int index)
{
    float resoulution_in = ui->imageRvalue->value();
    if(ui->imageResCombo->currentIndex() == PPM)
        resoulution_in = resoulution_in * CM_IN_INCH;

    if(index == PX)
    {
        currentUnit = index;
        ui->imageWvalue->setValue(width_px);
        ui->imageHvalue->setValue(height_px);
        ui->imageWvalue->setDecimals(0);
        ui->imageHvalue->setDecimals(0);
    }
    else if(index == CM)
    {
        currentUnit = index;
        ui->imageWvalue->setDecimals(2);
        ui->imageHvalue->setDecimals(2);
        ui->imageWvalue->setValue(width_px / resoulution_in * CM_IN_INCH);
        ui->imageHvalue->setValue(height_px / resoulution_in * CM_IN_INCH);
    }
    else if(index == IN)
    {
        currentUnit = index;
        ui->imageWvalue->setDecimals(2);
        ui->imageHvalue->setDecimals(2);
        ui->imageWvalue->setValue(width_px / resoulution_in);
        ui->imageHvalue->setValue(height_px / resoulution_in);
    }
}

void NewDialog::on_imageRvalue_valueChanged(double value)
{
    Q_UNUSED(value)
    on_imageWHcombo_currentIndexChanged(currentUnit);
}

void NewDialog::on_imageHvalue_valueChanged(double value)
{
    Q_UNUSED(value)
    float resoulution_in = ui->imageRvalue->value();
    if(ui->imageResCombo->currentIndex() == PPM)
        resoulution_in = resoulution_in * CM_IN_INCH;

    if(currentUnit == PX)
    {
        height_px = ui->imageHvalue->value();
    }
    else if(currentUnit == CM)
    {
        height_px = ui->imageHvalue->value() * resoulution_in / CM_IN_INCH;
    }
    else if(currentUnit == IN)
    {
        height_px = ui->imageHvalue->value() * resoulution_in;
    }

    ui->pixelHvalue->setValue(height_px);
    ui->memoryValue->setValue((((width_px * height_px) * 3)/1024)/1024);

    if(ui->lockedRatioButton->isChecked())
    {
        if(ui->imageHvalue->hasFocus())
            ui->imageWvalue->setValue(ui->imageHvalue->value() * imageRatio); //width = height * imageRatio
    }
    else
    {
        imageRatio = (width_px/height_px);
    }
}

void NewDialog::on_imageWvalue_valueChanged(double value)
{
    Q_UNUSED(value)
    float resoulution_in = ui->imageRvalue->value();
    if(ui->imageResCombo->currentIndex() == PPM)
        resoulution_in = resoulution_in * CM_IN_INCH;

    if(currentUnit == PX)
    {
        width_px = ui->imageWvalue->value();
    }
    else if(currentUnit == CM)
    {
        width_px = ui->imageWvalue->value() * resoulution_in / CM_IN_INCH;
    }
    else if(currentUnit == IN)
    {
        width_px = ui->imageWvalue->value() * resoulution_in;
    }
    ui->pixelWvalue->setValue(width_px);
    ui->memoryValue->setValue((((width_px * height_px) * 3)/1024)/1024);

    if(ui->lockedRatioButton->isChecked()) {
        if(ui->imageWvalue->hasFocus())
            ui->imageHvalue->setValue(ui->imageWvalue->value() / imageRatio); //height = width / imageRatio
    }
    else
    {
        imageRatio = (width_px/height_px);
    }
}

void NewDialog::setMode(Mode mode)
{
   if(mode == ResizeImage)
   {
        ui->lockedRatioButton->setVisible(true);
        ui->imageRes->setVisible(true);
        ui->imageRvalue->setVisible(true);
        ui->imageResCombo->setVisible(true);
        ui->imagePreset->setVisible(true);
        ui->imagePresetCombo->setVisible(true);
        ui->imageResCombo->setVisible(true);
        ui->backgroundLabel->setVisible(false);
        ui->backgroundColorComboBox->setVisible(false);
        ui->backgroundColorComboBox_NewFile->setVisible(true);
        ui->positionLabel->setVisible(false);
        ui->positionWidget->setVisible(false);
   }
   else
   {
       ui->lockedRatioButton->setVisible(false);
       ui->imageRes->setVisible(false);
       ui->imageRvalue->setVisible(false);
       ui->imageResCombo->setVisible(false);
       ui->imagePreset->setVisible(false);
       ui->imagePresetCombo->setVisible(false);
       ui->imageResCombo->setVisible(false);
       ui->backgroundLabel->setVisible(true);
       ui->backgroundColorComboBox->setVisible(true);
       ui->backgroundColorNewLabel->setVisible(false);
       ui->backgroundColorComboBox_NewFile->setVisible(false);
       ui->positionLabel->setVisible(true);
       ui->positionWidget->setVisible(true);
   }
}

void NewDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(!ui->backgroundColorComboBox->isVisible())
    {
        e->accept();
        return;
    }

    if(e->x() < ui->backgroundColorComboBox->width() - 20)
    {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this);
        if (selectedColor.isValid())
        {
            QPixmap pixmap(QSize(ui->backgroundColorComboBox->width(),ui->backgroundColorComboBox->height()));
            pixmap.fill(selectedColor);
            ui->backgroundColorComboBox->insertItem(0, QString(), pixmap);
            ui->backgroundColorComboBox->setCurrentIndex(0);
        }
        e->ignore();
    }
    else
    {
        e->accept();
    }
}

ImagePosition NewDialog::imagePosition()
{
    return ui->positionWidget->imagePosition();
}

/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// Dialog for creating a new image.

//#include <QDebug>

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QColorDialog>
#include <QSettings>

#include "NewDialog.h"
#include "ui_NewDialog.h"

#include "../Settings.h"

enum {PPM, PPI};
enum {PX, CM, IN};
enum {x0y0, x100y100, x640y480, x800y600, x1024y768, x1366y768, x1600y900, x1920y1080, x2560y1440, x3840y2160};

#define CM_IN_INCH 2.54f

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    // Set default values
    original_width_px = 0;
    original_height_px = 0;
    currentUnit = SETTINGS->getUnit();
    ui->imageResCombo->setCurrentIndex(PPI);
    ui->imageWHcombo->setCurrentIndex(currentUnit);
    // Standard PPI
    ui->imageRvalue->setValue(96.0);
    ui->pixelWvalue->setValue(width_px);
    ui->pixelHvalue->setValue(height_px);
    ui->memoryValue->setValue((((width_px * height_px) * 3)/1024)/1024);
    ui->lockedRatioButton->setChecked(true);

    // Setup Colour combobox colours
    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->backgroundColorComboBox->width(),ui->backgroundColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->backgroundColorComboBox->addItem(QString(), pixmap);
        ui->backgroundColorComboBox_NewFile->addItem(QString(), pixmap);
    }
    // Click handlers
    ui->backgroundColorComboBox->setOnClickHandler(this);
    ui->backgroundColorComboBox_NewFile->setOnClickHandler(this);
    ui->backgroundColourEnabled->setChecked(true);
    // Order for tabbing
    setTabOrder(ui->imageWvalue, ui->imageHvalue);
    ui->imageWvalue->setFocus();

    if(SETTINGS->getMemParamsEnabled() && ResizeImage != 1)
    {
        //Read Dialog settings
        readSettings(this);
    }
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
    original_width_px = size.width();
    original_height_px =size.height();
    ui->imageWvalue->setValue(size.width());
    ui->imageHvalue->setValue(size.height());
}

QColor NewDialog::backgroundColor() const
{
    QImage img = (QImage)ui->backgroundColorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

bool NewDialog::backgroundColorEnabled()
{
    return ui->backgroundColourEnabled->isChecked();
}

void NewDialog::on_buttonBox_accepted()
{
    m_chosenSize.setWidth(width_px);
    m_chosenSize.setHeight(height_px);

    if(SETTINGS->getMemParamsEnabled() && ResizeImage != 1)
    {
        //Read Dialog settings
        writeSettings(this);
    }
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
    else if (index == x2560y1440)
    {
        width_px = 2560;
        height_px = 1440;
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
    Q_UNUSED(value);
    if(original_width_px>0)
    {
        imageRatio = (original_width_px/original_height_px);
    }
    else
    {
        imageRatio = (width_px/height_px);
    }
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
}

void NewDialog::on_imageWvalue_valueChanged(double value)
{
    Q_UNUSED(value);
    if(original_width_px>0)
    {
        imageRatio = (original_width_px/original_height_px);
    }
    else
    {
        imageRatio = (width_px/height_px);
    }
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
        ui->backgroundColorComboBox_NewFile->setVisible(false);
        ui->backgroundColorNewLabel->setVisible(false);
        ui->positionLabel->setVisible(false);
        ui->positionWidget->setVisible(false);
        ui->backgroundColourEnabled->setVisible(false);
   }
   else if (mode == NewImage) {
       ui->positionLabel->setVisible(false);
       ui->positionWidget->setVisible(false);
       ui->lockedRatioButton->setChecked(false);
       ui->backgroundLabel->setVisible(false);
       ui->backgroundColorComboBox->setVisible(false);
       ui->backgroundColourEnabled->setVisible(true);
   }
   else
   {
       ui->lockedRatioButton->setVisible(true);
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
       ui->backgroundColourEnabled->setVisible(false);
   }
}

void NewDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(obj == ui->backgroundColorComboBox)
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
    else if(obj == ui->backgroundColorComboBox_NewFile)
    {
        if(!ui->backgroundColorComboBox_NewFile->isVisible())
        {
            e->accept();
            return;
        }

        if(e->x() < ui->backgroundColorComboBox_NewFile->width() - 20)
        {
            QColor selectedColor = QColorDialog::getColor(Qt::white, this);
            if (selectedColor.isValid())
            {
                QPixmap pixmap(QSize(ui->backgroundColorComboBox_NewFile->width(),ui->backgroundColorComboBox_NewFile->height()));
                pixmap.fill(selectedColor);
                ui->backgroundColorComboBox_NewFile->insertItem(0, QString(), pixmap);
                ui->backgroundColorComboBox_NewFile->setCurrentIndex(0);
            }
            e->ignore();
        }
        else
        {
            e->accept();
        }
    }
}

ImagePosition NewDialog::imagePosition()
{
    return ui->positionWidget->imagePosition();
}

void NewDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("width", width_px);
    settings.setValue("height", height_px);
    settings.endGroup();
}

void NewDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        ui->imageWvalue->setValue(settings.value("width").toFloat());
        ui->imageHvalue->setValue(height_px = settings.value("height").toFloat());
    }
    settings.endGroup();
}

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

// Image position widget for the PhotoFlare application.

#include "imagepositionwidget.h"
#include "ui_imagepositionwidget.h"
//#include <QDebug>

ImagePositionWidget::ImagePositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePositionWidget)
{
    ui->setupUi(this);
    iconSepia = ":/icons/assets/icons/toolbar2/sepia.png";
    iconGray = ":/icons/assets/icons/toolbar2/grayscale.png";
    on_pushButton_5_clicked();
}

ImagePositionWidget::~ImagePositionWidget()
{
    delete ui;
}

void ImagePositionWidget::resetAllButtons()
{
    foreach(QObject* obj,this->children())
    {
        QPushButton* pushButton = qobject_cast<QPushButton*>(obj);

        if(pushButton && pushButton->isChecked())
        {
            pushButton->setChecked(false);
            pushButton->setIcon(QIcon(iconGray));
        }
    }
}

void ImagePositionWidget::on_pushButton_clicked()
{
    resetAllButtons();
    ui->pushButton->setChecked(true);
    ui->pushButton->setIcon(QIcon(iconSepia));
    position = LeftTop;
}

void ImagePositionWidget::on_pushButton_2_clicked()
{
    resetAllButtons();
    ui->pushButton_2->setChecked(true);
    ui->pushButton_2->setIcon(QIcon(iconSepia));
    position = CenterTop;
}

void ImagePositionWidget::on_pushButton_3_clicked()
{
    resetAllButtons();
    ui->pushButton_3->setChecked(true);
    ui->pushButton_3->setIcon(QIcon(iconSepia));
    position = RightTop;
}

void ImagePositionWidget::on_pushButton_4_clicked()
{
    resetAllButtons();
    ui->pushButton_4->setChecked(true);
    ui->pushButton_4->setIcon(QIcon(iconSepia));
    position = LeftCenter;
}

void ImagePositionWidget::on_pushButton_5_clicked()
{
    resetAllButtons();
    ui->pushButton_5->setChecked(true);
    ui->pushButton_5->setIcon(QIcon(iconSepia));
    position = CenterCenter;
}

void ImagePositionWidget::on_pushButton_6_clicked()
{
    resetAllButtons();
    ui->pushButton_6->setChecked(true);
    ui->pushButton_6->setIcon(QIcon(iconSepia));
    position = RightCenter;
}

void ImagePositionWidget::on_pushButton_7_clicked()
{
    resetAllButtons();
    ui->pushButton_7->setChecked(true);
    ui->pushButton_7->setIcon(QIcon(iconSepia));
    position = LeftBottom;
}

void ImagePositionWidget::on_pushButton_8_clicked()
{
    resetAllButtons();
    ui->pushButton_8->setChecked(true);
    ui->pushButton_8->setIcon(QIcon(iconSepia));
    position = CenterBottom;
}

void ImagePositionWidget::on_pushButton_9_clicked()
{
    resetAllButtons();
    ui->pushButton_9->setChecked(true);
    ui->pushButton_9->setIcon(QIcon(iconSepia));
    position = RightBottom;
}

/*

  Image position widget for the PhotoFlare application.

*/

#include "imagepositionwidget.h"
#include "ui_imagepositionwidget.h"
//#include <QDebug>

ImagePositionWidget::ImagePositionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagePositionWidget)
{
    ui->setupUi(this);
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
            pushButton->setIcon(QIcon(":/icons/icons/grayscale.png"));
        }
    }
}

void ImagePositionWidget::on_pushButton_clicked()
{
    resetAllButtons();
    ui->pushButton->setChecked(true);
    ui->pushButton->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = LeftTop;
}

void ImagePositionWidget::on_pushButton_2_clicked()
{
    resetAllButtons();
    ui->pushButton_2->setChecked(true);
    ui->pushButton_2->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = CenterTop;
}

void ImagePositionWidget::on_pushButton_3_clicked()
{
    resetAllButtons();
    ui->pushButton_3->setChecked(true);
    ui->pushButton_3->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = RightTop;
}

void ImagePositionWidget::on_pushButton_4_clicked()
{
    resetAllButtons();
    ui->pushButton_4->setChecked(true);
    ui->pushButton_4->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = LeftCenter;
}

void ImagePositionWidget::on_pushButton_5_clicked()
{
    resetAllButtons();
    ui->pushButton_5->setChecked(true);
    ui->pushButton_5->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = CenterCenter;
}

void ImagePositionWidget::on_pushButton_6_clicked()
{
    resetAllButtons();
    ui->pushButton_6->setChecked(true);
    ui->pushButton_6->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = RightCenter;
}

void ImagePositionWidget::on_pushButton_7_clicked()
{
    resetAllButtons();
    ui->pushButton_7->setChecked(true);
    ui->pushButton_7->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = LeftBottom;
}

void ImagePositionWidget::on_pushButton_8_clicked()
{
    resetAllButtons();
    ui->pushButton_8->setChecked(true);
    ui->pushButton_8->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = CenterBottom;
}

void ImagePositionWidget::on_pushButton_9_clicked()
{
    resetAllButtons();
    ui->pushButton_9->setChecked(true);
    ui->pushButton_9->setIcon(QIcon(":/icons/icons/sepia.png"));
    position = RightBottom;
}

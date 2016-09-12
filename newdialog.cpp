#include "newdialog.h"
#include "ui_newdialog.h"
#include <QTabWidget>
#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

newDialog::newDialog(QWidget *parent, Ui::MainWindow* window) :
    QDialog(parent),
    ui(new Ui::newDialog)
{
    ui->setupUi(this);
    uimain = window;

    ui->imageWHcombo->addItem("Pixels");
    ui->imageWHcombo->addItem("Cm");
    ui->imageWHcombo->addItem("Inches");

    ui->imageResCombo->addItem("Pixels/Cm");
    ui->imageResCombo->addItem("Pixels/Inch");

    ui->imagePresetCombo->addItem("100 x 100 pixels");
    ui->imagePresetCombo->addItem("640 x 480 pixels");
    ui->imagePresetCombo->addItem("800 x 600 pixels");
}

newDialog::~newDialog()
{
    delete ui;
}

void newDialog::on_buttonBox_accepted()
{
    int width = ui->imageWvalue->value();
    int height = ui->imageHvalue->value();
    QImage *img = new QImage(width, height, QImage::Format_RGB888);
    img->fill(QColor(Qt::white).rgb());

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    view->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    scene->addItem(item);
    view->show();

    uimain->tabWidget->addTab(view, "Untitled");
}

void newDialog::on_imagePresetCombo_currentIndexChanged(int index)
{
    if(index==0) {
        ui->imageWvalue->setValue(100);
        ui->imageHvalue->setValue(100);
    }
    if(index==1) {
        ui->imageWvalue->setValue(640);
        ui->imageHvalue->setValue(480);
    }
    if(index==2) {
        ui->imageWvalue->setValue(800);
        ui->imageHvalue->setValue(600);
    }
}

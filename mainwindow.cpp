#include "mainwindow.h"
#include "newdialog.h"
#include "textdialog.h"
#include "prefsdialog.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    bool maximize = false;
    QSettings settings("Startup");
    maximize = settings.value("maximizeOnStartup").toBool();
    if(maximize==true) {
        this->setWindowState(Qt::WindowMaximized);
    }
    if(maximize==false) {
        this->setFixedSize(settings.value("manualWindowWidth").toInt(),settings.value("manualWindowHeight").toInt());
    }
    ui->setupUi(this);

    QComboBox* zoomCombo = new QComboBox;
    zoomCombo->setFocusPolicy( Qt::NoFocus );
    QStringList list(QStringList() <<"100%"<<"90%"<<"80%"<<"70%"<<"60%"<<"50%"<<"40%"<<"30%"<<"20%"<<"10%");
    zoomCombo->addItems(list);
    ui->mainToolBar->addWidget(zoomCombo);

    toolSelected = "pointer";
    ui->toolButtonPointer->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionNew_triggered()
{
    newDialog dialog(this, ui);
    dialog.exec();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File..."),
    QString(), tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));
    qDebug() << "filename" << fileName;

    QImage *img = new QImage();
    img->load(fileName);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);
    view->setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
    scene->addItem(item);
    view->show();
    ui->tabWidget->addTab(view, "Untitled");
}

void MainWindow::on_actionSave_triggered()
{
    //Need to check fileName from currentTab
    QString fileName;

    if (fileName.isEmpty())
       return on_actionSave_As_triggered();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr(".png (*.png);; .jpg (*.jpg *.jpeg);;.gif (*.gif);;All Files (*)"));
    //QPixmap pixMap = QPixmap::grabWidget(scene);
    //pixMap.save(fileName);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_actionText_triggered()
{
    textDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionPreferences_triggered()
{
    prefsDialog dialog(this);
    dialog.exec();
}

void MainWindow::clearToolpalette()
{
    ui->toolButtonPointer->setChecked(false);
    ui->toolButtonDropper->setChecked(false);
    ui->toolButtonWand->setChecked(false);
    ui->toolButtonLine->setChecked(false);
    ui->toolButtonPaintBucket->setChecked(false);
    ui->toolButtonSprayCan->setChecked(false);
    ui->toolButtonPaintBrush->setChecked(false);
    ui->toolButtonPaintBrushAdv->setChecked(false);
    ui->toolButtonStamp->setChecked(false);
    ui->toolButtonBlur->setChecked(false);
}

void MainWindow::on_toolButtonPointer_clicked()
{
        clearToolpalette();
        toolSelected = "pointer";
        ui->toolButtonPointer->setChecked(true);
}

void MainWindow::on_toolButtonDropper_clicked()
{
        clearToolpalette();
        toolSelected = "dropper";
        ui->toolButtonDropper->setChecked(true);
}

void MainWindow::on_toolButtonWand_clicked()
{
        clearToolpalette();
        toolSelected = "wand";
        ui->toolButtonWand->setChecked(true);
}

void MainWindow::on_toolButtonLine_clicked()
{
        clearToolpalette();
        toolSelected = "line";
        ui->toolButtonLine->setChecked(true);
}

void MainWindow::on_toolButtonPaintBucket_clicked()
{
        clearToolpalette();
        toolSelected = "paintBucket";
        ui->toolButtonPaintBucket->setChecked(true);
}

void MainWindow::on_toolButtonSprayCan_clicked()
{
        clearToolpalette();
        toolSelected = "sprayCan";
        ui->toolButtonSprayCan->setChecked(true);
}

void MainWindow::on_toolButtonPaintBrush_clicked()
{
        clearToolpalette();
        toolSelected = "paintBrush";
        ui->toolButtonPaintBrush->setChecked(true);
}

void MainWindow::on_toolButtonPaintBrushAdv_clicked()
{
        clearToolpalette();
        toolSelected = "paintBrushAdv";
        ui->toolButtonPaintBrushAdv->setChecked(true);
}

void MainWindow::on_toolButtonStamp_clicked()
{
        clearToolpalette();
        toolSelected = "stamp";
        ui->toolButtonStamp->setChecked(true);
}

void MainWindow::on_toolButtonBlur_clicked()
{
        clearToolpalette();
        toolSelected = "blur";
        ui->toolButtonBlur->setChecked(true);
}

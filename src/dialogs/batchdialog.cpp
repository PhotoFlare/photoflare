/*

  Batch dialog class for the PhotoFlare application.

*/

//#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QSettings>

#include "batchdialog.h"
#include "ui_batchdialog.h"
#include "NewDialog.h"
#include "../Settings.h"

enum {Pixels, Percent};

class BatchDialogPrivate
{
public:
    QStringList fileList;
    QStringList filterList;
    QString openDir;
    QString outDir;
    QString outFormat;
    double imageRatio;
};

batchDialog::batchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::batchDialog),
    d(new BatchDialogPrivate)
{
    ui->setupUi(this);
    setFixedSize(size());

    QStringList formats;
    formats << tr("*.png");
    formats << tr("*.jpg");
    formats << tr("*.gif");

    QStringList outputformats;
    outputformats << tr(".png");
    outputformats << tr(".jpg");
    outputformats << tr(".bmp");

    ui->comboBox_2->addItems(formats);
    ui->outFormat->addItems(outputformats);

    QStringList filters;
    filters << tr("Oil");
    filters << tr("Charcoal");
    filters << tr("Swirl");
    filters << tr("Solarize");
    filters << tr("Wave");
    filters << tr("Implode");
    filters << tr("Soften");
    filters << tr("Blur");
    filters << tr("Sharpen");
    filters << tr("Reinforce");
    filters << tr("Grayscale");
    filters << tr("Old Photo");
    filters << tr("Sepia");
    filters << tr("Implode");
    filters << tr("Explode");
    filters << tr("Simple Frame");
    filters << tr("3D Frame");
    filters << tr("Normalize");
    filters << tr("Motion Blur");
    filters << tr("Crop to center");
    filters << tr("Equalize colours");
    filters << tr("Monochrome edges");
    filters << tr("Gaussian noise");
    filters << tr("Drop shadow");
    filters << tr("Opacity");
    ui->listWidget_2->addItems(filters);

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->backgroundColorComboBox->width(),ui->backgroundColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->backgroundColorComboBox->addItem(QString(), pixmap);
    }

    ui->backgroundColorComboBox->setOnClickHandler(this);
}

batchDialog::~batchDialog()
{
    delete ui;
}

void batchDialog::done(int r)
{
    if(QDialog::Accepted == r)
    {
        if(ui->listWidget->count() > 0 && ui->lineEdit_3->text().size() > 0)
        {
            QDialog::done(r);
            return;
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Please add files to the batch file list and set output folder."));
            msgBox.exec();
            return;
        }
    }
    else
    {
        QDialog::done(r);
        return;
    }
}

void batchDialog::on_listWidget_2_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget_2->item(index.row());
    ui->listWidget_3->addItem(item->text());
    d->filterList.append(item->text());
    ui->listWidget_2->removeItemWidget(item);
    delete item;
}

void batchDialog::on_listWidget_3_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget_3->item(index.row());
    ui->listWidget_2->addItem(item->text());
    ui->listWidget_3->removeItemWidget(item);
    d->filterList.removeOne(item->text());
    delete item;
}

QStringList batchDialog::fileList()
{
    return d->fileList;
}

QStringList batchDialog::filterList()
{
    return d->filterList;
}

QString batchDialog::outDir()
{
    return d->outDir;
}

QString batchDialog::outputFormat()
{
    return d->outFormat;
}

void batchDialog::on_tabWidget_currentChanged(int index)
{
    if(index == 5)
    {
        ui->plainTextEdit->clear();
        ui->plainTextEdit->insertPlainText("Input files:\n");
        foreach (QString file, d->fileList) {
            ui->plainTextEdit->insertPlainText("  " + file + "\n");
        }
        ui->plainTextEdit->insertPlainText("\n");
        ui->plainTextEdit->insertPlainText("Output folder: " + d->outDir + "\n");
        ui->plainTextEdit->insertPlainText("\n");
        ui->plainTextEdit->insertPlainText("Filters: \n");
        foreach (QString filter, d->filterList) {
            ui->plainTextEdit->insertPlainText("  " + filter + "\n");
        }
    }
}

void batchDialog::onFinished()
{
    ui->tabWidget->setCurrentIndex(5);
    ui->plainTextEdit->insertPlainText("\n");
    ui->plainTextEdit->insertPlainText("Batch processing finished successfully !");
}

void batchDialog::on_imageSizeCheckBox_clicked(bool checked)
{
    ui->imageWidth->setEnabled(checked);
    ui->imageHeight->setEnabled(checked);
    ui->imageUnit->setEnabled(checked);
    ui->aspectRatioCheckBox->setEnabled(checked);
    ui->imageWvalue->setEnabled(checked);
    ui->imageHvalue->setEnabled(checked);
    ui->imageWHcombo->setEnabled(checked);
}

void batchDialog::on_canvasSizeCheckBox_clicked(bool checked)
{
    ui->canvasWidth->setEnabled(checked);
    ui->canvasHeight->setEnabled(checked);
    ui->backgroundLabel->setEnabled(checked);
    ui->positionLabel->setEnabled(checked);
    ui->canvasWvalue->setEnabled(checked);
    ui->canvasHvalue->setEnabled(checked);
    ui->backgroundColorComboBox->setEnabled(checked);
    ui->positionWidget->setEnabled(checked);
}

void batchDialog::on_imageWHcombo_currentIndexChanged(int index)
{
    if(index == Pixels)
    {
        d->imageRatio = 800.0f / 600.0f;
        ui->imageWvalue->setValue(800);
        ui->imageHvalue->setValue(600);
    }
    else
    {
        d->imageRatio = 1;
        ui->imageWvalue->setValue(100);
        ui->imageHvalue->setValue(100);
    }
}

void batchDialog::on_imageWvalue_valueChanged(double value)
{
    if(ui->aspectRatioCheckBox->isChecked())
    {
        ui->imageHvalue->setValue(value / d->imageRatio);
    }
    else
    {
        d->imageRatio = value / ui->imageHvalue->value();
    }
}

void batchDialog::on_imageHvalue_valueChanged(double value)
{
    if(ui->aspectRatioCheckBox->isChecked())
    {
        ui->imageWvalue->setValue(value * d->imageRatio);
    }
    else
    {
        d->imageRatio = ui->imageWvalue->value() / value;
    }
}

void batchDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
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

bool batchDialog::changeImageSize() const
{
    return ui->imageSizeCheckBox->isChecked();
}

bool batchDialog::changeCanvasSize() const
{
    return ui->canvasSizeCheckBox->isChecked();
}

QSize batchDialog::imageSize() const
{
    return QSize(ui->imageWvalue->value(), ui->imageHvalue->value());
}

char batchDialog::imageSizeUnits() const
{
    return ui->imageWHcombo->currentIndex();
}

QSize batchDialog::canvasSize() const
{
    return QSize(ui->canvasWvalue->value(), ui->canvasHvalue->value());
}

QColor batchDialog::backgroundColor() const
{
    QImage img = (QImage)ui->backgroundColorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

ImagePosition batchDialog::imagePosition() const
{
    return ui->positionWidget->imagePosition();
}

int batchDialog::brightness() const
{
    return ui->brightnessSlider->value();
}

int batchDialog::brightnessChannel() const
{
    return ui->brightnessChannel->currentIndex();
}

int batchDialog::gamma() const
{
    return ui->gammaSlider->value();
}

int batchDialog::gammaChannel() const
{
    return ui->gammaChannel->currentIndex();
}

int batchDialog::contrast() const
{
    return ui->contrastSlider->value();
}

int batchDialog::contrastChannel() const
{
    return ui->contrastChannel->currentIndex();
}

int batchDialog::saturation() const
{
    return ui->saturationSlider->value();
}

int batchDialog::saturationChannel() const
{
    return ui->SaturationChannel->currentIndex();
}

void batchDialog::on_rotateCheckBox_clicked(bool checked)
{
    ui->rotate90radioButton->setEnabled(checked);
    ui->rotate90ccwRadioButton->setEnabled(checked);
    ui->rotate180RadioButton->setEnabled(checked);
}

void batchDialog::on_flipCheckBox_clicked(bool checked)
{
    ui->flipVerticalRadioButton->setEnabled(checked);
    ui->flipHorizontalRadioButton->setEnabled(checked);
}

char batchDialog::rotate() const
{
    if(!ui->rotateCheckBox->isChecked())
        return 0;
    if(ui->rotate90radioButton->isChecked())
        return Rotate90CW;
    if(ui->rotate90ccwRadioButton->isChecked())
        return Rotate90CCW;
    if(ui->rotate180RadioButton->isChecked())
        return Rotate180;
}

char batchDialog::flip() const
{
    if(!ui->flipCheckBox->isChecked())
        return 0;
    if(ui->flipVerticalRadioButton)
        return FlipVertical;
    if(ui->flipHorizontalRadioButton)
        return FlipHorizontal;
}

void batchDialog::on_addFilesButton_clicked()
{
        if(SETTINGS->getMemParamsEnabled() == true)
        {
            readSettings(this);
        }
        else
        {
           d->openDir = QString();
        }
        d->fileList = QFileDialog::getOpenFileNames(this, tr("Select Files"),d->openDir, tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));

        if(d->fileList.length()>0)
        {
            if(SETTINGS->getMemParamsEnabled() == true)
            {
                QDir dir = QFileInfo(d->fileList[0]).absoluteDir();
                d->openDir = dir.absolutePath();
                writeSettings(this);
            }
            ui->listWidget->clear();

            for(QString file : d->fileList)
            {
                QListWidgetItem *itm = new QListWidgetItem(QFileInfo(file).fileName() + "\r\n" + file);
                itm->setSizeHint(QSize(64,70));
                itm->setIcon(QIcon(file));
                ui->listWidget->setIconSize(QSize(64,64));
                ui->listWidget->addItem(itm);
            }
        }
}

void batchDialog::on_outFolderPushButton_clicked()
{
    d->outDir = QFileDialog::getExistingDirectory();
    ui->lineEdit_3->setText(d->outDir);
}

void batchDialog::on_brightnessSlider_valueChanged(int value)
{
    ui->brightnessValue->setText(QString::number(value) + QString("%"));
}

void batchDialog::on_contrastSlider_valueChanged(int value)
{
    ui->contrastValue->setText(QString::number(value) + QString("%"));
}

void batchDialog::on_saturationSlider_valueChanged(int value)
{
    ui->saturationValue->setText(QString::number(value) + QString("%"));
}

void batchDialog::on_gammaSlider_valueChanged(int value)
{
    ui->gammaValue->setText(QString::number((float)value / (float)100, 'f', 2));
}

void batchDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("opendir", d->openDir);
    settings.endGroup();
}

void batchDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        d->openDir = settings.value("opendir").toString();
    }
    settings.endGroup();
}

void batchDialog::on_outFormat_currentIndexChanged(int index)
{
    d->outFormat = ui->outFormat->currentText();
}

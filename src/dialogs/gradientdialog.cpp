/*

  Gradient dialog class for the PhotoFlare application.

*/

#include <QMouseEvent>
#include <QColorDialog>
#include <QSettings>

#include "gradientdialog.h"
#include "ui_gradientdialog.h"

GradientDialog::GradientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->startColorComboBox->width(),ui->startColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->startColorComboBox->addItem(QString(), pixmap);
    }

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->stopColorComboBox->width(),ui->stopColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->stopColorComboBox->addItem(QString(), pixmap);
    }

    ui->startColorComboBox->setOnClickHandler(this);
    ui->stopColorComboBox->setOnClickHandler(this);

    ui->opacityValue->setText("50%");
    ui->opacityValue_2->setText("50%");

    readSettings(this);
}

GradientDialog::~GradientDialog()
{
    delete ui;
}

void GradientDialog::applyDirection()
{
    if(ui->directionN->isChecked())
    {
        m_direction = N;
    }
    else if(ui->directionNE->isChecked())
    {
        m_direction = NE;
    }
    else if(ui->directionE->isChecked())
    {
        m_direction = E;
    }
    else if(ui->directionSE->isChecked())
    {
        m_direction = SE;
    }
    else if(ui->directionS->isChecked())
    {
        m_direction = S;
    }
    else if(ui->directionSW->isChecked())
    {
        m_direction = SW;
    }
    else if(ui->directionW->isChecked())
    {
        m_direction = W;
    }
    else if(ui->directionNW->isChecked())
    {
        m_direction = NW;
    }
}

void GradientDialog::on_buttonBox_accepted()
{
    applyDirection();
    writeSettings(this);
}

void GradientDialog::on_startOpacity_valueChanged(int value)
{
    ui->opacityValue->setText(QString("%1%").arg(value));
}

void GradientDialog::on_stopOpacity_valueChanged(int value)
{
    ui->opacityValue_2->setText(QString("%1%").arg(value));
}

void GradientDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(e->x() < obj->width() - 20)
    {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this);
        if (selectedColor.isValid())
        {
            QPixmap pixmap(QSize(obj->width(),obj->height()));
            pixmap.fill(selectedColor);
            obj->insertItem(0, QString(), pixmap);
            obj->setCurrentIndex(0);
        }
        e->ignore();
    }
    else
    {
        e->accept();
    }
}

QColor GradientDialog::startColor()
{
    QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return QColor(color.red(), color.green(), color.blue(), ui->startOpacity->value()*255.0/100.0);
}

QColor GradientDialog::stopColor()
{
    if(ui->duatone->isChecked())
    {
        QImage img = (QImage)ui->stopColorComboBox->currentData().value<QImage>();
        QColor color = img.pixel(0,0);
        return QColor(color.red(), color.green(), color.blue(), ui->stopOpacity->value()*255.0/100.0);
    }
    else
    {
        QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
        QColor color = img.pixel(0,0);
        return QColor(color.red(), color.green(), color.blue(), 0);
    }
}

QColor GradientDialog::startColorName()
{
    QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return color.name();
}

QColor GradientDialog::stopColorName()
{
    QImage img = (QImage)ui->stopColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return color.name();
}

void GradientDialog::on_radioButton_2_toggled(bool checked)
{
    ui->stopColorGroupBox->setEnabled(!checked);
}

void GradientDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("size", window->size());
    settings.setValue("startcolor", startColorName());
    settings.setValue("stopcolor", stopColorName());
    settings.setValue("direction", m_direction);
    settings.setValue("startopacity", ui->startOpacity->value());
    settings.setValue("stopopacity", ui->stopOpacity->value());
    settings.endGroup();
}

void GradientDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        window->resize(settings.value("size").toSize());

        QPixmap pixmap(QSize(ui->startColorComboBox->width(),ui->startColorComboBox->height()));
        pixmap.fill(settings.value("startcolor").value<QColor>());
        ui->startColorComboBox->insertItem(0, QString(), pixmap);
        ui->startColorComboBox->setCurrentIndex(0);
        ui->startOpacity->setValue(settings.value("startopacity").toInt());

        QPixmap pixmap2(QSize(ui->stopColorComboBox->width(),ui->stopColorComboBox->height()));
        pixmap2.fill(settings.value("stopcolor").value<QColor>());
        ui->stopColorComboBox->insertItem(0, QString(), pixmap2);
        ui->stopColorComboBox->setCurrentIndex(0);
        ui->stopOpacity->setValue(settings.value("stopopacity").toInt());

    }
    settings.endGroup();
}

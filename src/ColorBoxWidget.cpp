/*

  Widget to choose colors.

*/

#include "colorboxwidget.h"
#include "ui_ColorBoxWidget.h"
#include <QColorDialog>

ColorBoxWidget::ColorBoxWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorBoxWidget)
{
    ui->setupUi(this);

    ui->primaryColorLabel->setAutoFillBackground(true);
    ui->secondaryColorLabel->setAutoFillBackground(true);
    setColor(Qt::black, ui->primaryColorLabel);
    setColor(Qt::red, ui->secondaryColorLabel);

    ui->primaryColorLabel->installEventFilter(this);
    ui->secondaryColorLabel->installEventFilter(this);
}

ColorBoxWidget::~ColorBoxWidget()
{
    delete ui;
}

QColor ColorBoxWidget::primaryColor() const
{
    return getColorFromLabel(ui->primaryColorLabel);
}

QColor ColorBoxWidget::secondaryColor() const
{
    return getColorFromLabel(ui->secondaryColorLabel);
}

void ColorBoxWidget::setPrimaryColor(const QColor &color)
{
    setColor(color, ui->primaryColorLabel);
    emit primaryColorChanged(color);
}

void ColorBoxWidget::setSecondaryColor(const QColor &color)
{
    setColor(color, ui->secondaryColorLabel);
    emit secondaryColorChanged(color);
}

bool ColorBoxWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        if (obj == ui->primaryColorLabel || obj == ui->secondaryColorLabel) {
            showColorDialog(static_cast<QWidget *>(obj));
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void ColorBoxWidget::showColorDialog(QWidget *colorLabel)
{
    QColor selectedColor = QColorDialog::getColor(getColorFromLabel(colorLabel), this);
    if (selectedColor.isValid()) {
        setColor(selectedColor, colorLabel);

        if (colorLabel == ui->primaryColorLabel)
            emit primaryColorChanged(selectedColor);
        else
            emit secondaryColorChanged(selectedColor);
    }
}

void ColorBoxWidget::setColor(const QColor &color, QWidget *colorLabel)
{
    QPalette palette = colorLabel->palette();
    palette.setColor(colorLabel->backgroundRole(), color);
    colorLabel->setPalette(palette);
}

QColor ColorBoxWidget::getColorFromLabel(QWidget *colorLabel) const
{
    return colorLabel->palette().color(colorLabel->backgroundRole());
}

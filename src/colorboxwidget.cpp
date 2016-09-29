/*

  Widget to choose colors.

*/

#include "colorboxwidget.h"
#include "ui_ColorBoxWidget.h"
#include <QColorDialog>
#include <QMouseEvent>

ColorBoxWidget::ColorBoxWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorBoxWidget)
{
    ui->setupUi(this);

    setColor(Qt::black, ui->primaryColorLabel);
    setColor(Qt::red, ui->secondaryColorLabel);

    ui->primaryColorLabel->installEventFilter(this);
    ui->secondaryColorLabel->installEventFilter(this);

    foreach(QObject* obj, ui->page->children()) {
        obj->installEventFilter(this);
    }

    ui->bwGradientLabel->installEventFilter(this);
    ui->bwGradientLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 black, stop: 1 white);");

    ui->spectrumLabel->installEventFilter(this);
    ui->spectrumLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 red, stop: 0.16 yellow, stop: 0.32 green, \
                                                                                                    stop: 0.48 cyan, stop: 0.64 blue, stop: 0.8 magenta, stop: 1 red);");
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

        if(obj->parent() == ui->page) {
            QMouseEvent *mevent = (QMouseEvent*)event;
            QColor color = getColorFromLabel((QLabel*)obj);
            if(mevent->button() == Qt::LeftButton) {
                setColor(color, ui->primaryColorLabel);
                emit primaryColorChanged(color);
            } else  {
                setColor(color, ui->secondaryColorLabel);
                emit secondaryColorChanged(color);
            }
            return true;
        }

        if(obj == ui->bwGradientLabel || obj == ui->spectrumLabel) {
            QMouseEvent *mevent = (QMouseEvent*)event;
            QLabel* label = (QLabel*)obj;
            QPixmap pxm = label->grab();
            QColor color = QColor(pxm.toImage().pixel(mevent->x(),mevent->y()));
            if(mevent->button() == Qt::LeftButton) {
                setColor(color, ui->primaryColorLabel);
                emit primaryColorChanged(color);
            } else  {
                setColor(color, ui->secondaryColorLabel);
                emit secondaryColorChanged(color);
            }
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

void ColorBoxWidget::on_swapColorButton_clicked()
{
    QColor primaryC = getColorFromLabel(ui->primaryColorLabel);
    QColor secondaryC = getColorFromLabel(ui->secondaryColorLabel);

    setColor(secondaryC, ui->primaryColorLabel);
    emit primaryColorChanged(secondaryC);
    setColor(primaryC, ui->secondaryColorLabel);
    emit secondaryColorChanged(primaryC);
}

void ColorBoxWidget::on_prevPage_clicked()
{
    int idx = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(idx == 0? 2 : idx-1);
}

void ColorBoxWidget::on_nextPage_clicked()
{
    ui->stackedWidget->setCurrentIndex((ui->stackedWidget->currentIndex() + 1) % 3);
}

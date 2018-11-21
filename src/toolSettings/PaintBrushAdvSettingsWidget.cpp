/*

  Widget for AdvancedPaintbrushTool settings.

*/

#include "PaintBrushAdvSettingsWidget.h"
#include "ui_PaintBrushAdvSettingsWidget.h"

PaintBrushAdvSettingsWidget::PaintBrushAdvSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintBrushAdvSettingsWidget)
{
    ui->setupUi(this);

    for(int i=1; i < 10; i++)
    {
        QPixmap pixmap = QPixmap (QString(":/brushes/assets/brushes/type_%1.png").arg(i));
        ui->comboBox->addItem(QString(), pixmap);
    }

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->pressureSlider, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->stepSlider, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->fadeCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
}

PaintBrushAdvSettingsWidget::~PaintBrushAdvSettingsWidget()
{
    delete ui;
}

QPixmap PaintBrushAdvSettingsWidget::brushPixmap()
{
    return ui->comboBox->itemData(ui->comboBox->currentIndex(), Qt::UserRole).value<QPixmap>();
}

int PaintBrushAdvSettingsWidget::brushWidth() const
{
    return ui->comboBox->height();
}

int PaintBrushAdvSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

bool PaintBrushAdvSettingsWidget::fade() const
{
    return ui->fadeCheckBox->isChecked();
}

int PaintBrushAdvSettingsWidget::step() const
{
    return ui->stepSlider->value();
}

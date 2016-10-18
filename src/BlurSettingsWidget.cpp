/*

  Widget for BlurTool settings.

*/


#include "BlurSettingsWidget.h"
#include "ui_BlurSettingsWidget.h"

BlurSettingsWidget::BlurSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlurSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &BlurSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &BlurSettingsWidget::settingsChanged);
}

BlurSettingsWidget::~BlurSettingsWidget()
{
    delete ui;
}

int BlurSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int BlurSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

/*

  Widget for SprayCanTool settings.

*/


#include "SprayCanSettingsWidget.h"
#include "ui_SprayCanSettingsWidget.h"

SprayCanSettingsWidget::SprayCanSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SprayCanSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->dispersionSlider, &QSlider::valueChanged, this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->rainbowCheckBox, &QCheckBox::clicked, this, &SprayCanSettingsWidget::settingsChanged);
}

SprayCanSettingsWidget::~SprayCanSettingsWidget()
{
    delete ui;
}

int SprayCanSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int SprayCanSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

int SprayCanSettingsWidget::dispersion() const
{
    return ui->dispersionSlider->value();
}

bool SprayCanSettingsWidget::rainbow() const
{
    return ui->rainbowCheckBox->isChecked();
}

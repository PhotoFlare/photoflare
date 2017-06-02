/*

  Widget for SmudgeTool settings.

*/

#include "SmudgeSettingsWidget.h"
#include "ui_smudgesettingswidget.h"

SmudgeSettingsWidget::SmudgeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmudgeSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SmudgeSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &SmudgeSettingsWidget::settingsChanged);
}

SmudgeSettingsWidget::~SmudgeSettingsWidget()
{
    delete ui;
}

int SmudgeSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int SmudgeSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

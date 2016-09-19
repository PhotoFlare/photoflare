/*

  Widget for paintbrush settings.

*/


#include "MagicWandSettingsWidget.h"
#include "ui_MagicWandSettingsWidget.h"

MagicWandSettingsWidget::MagicWandSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MagicWandSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->toleranceSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MagicWandSettingsWidget::settingsChanged);
    connect(ui->colorCheckBox, &QCheckBox::clicked, this, &MagicWandSettingsWidget::settingsChanged);
}

MagicWandSettingsWidget::~MagicWandSettingsWidget()
{
    delete ui;
}

int MagicWandSettingsWidget::tolerance() const
{
    return ui->toleranceSpinBox->value();
}

bool MagicWandSettingsWidget::color() const
{
    return ui->colorCheckBox->isChecked();
}

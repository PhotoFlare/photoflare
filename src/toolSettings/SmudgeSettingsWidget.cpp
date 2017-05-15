#include "SmudgeSettingsWidget.h"
#include "ui_smudgesettingswidget.h"

SmudgeSettingsWidget::SmudgeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmudgeSettingsWidget)
{
    ui->setupUi(this);
}

SmudgeSettingsWidget::~SmudgeSettingsWidget()
{
    delete ui;
}

int SmudgeSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

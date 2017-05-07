#include "erasersettingswidget.h"
#include "ui_erasersettingswidget.h"

EraserSettingsWidget::EraserSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EraserSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSlider, &QSlider::valueChanged, this, &EraserSettingsWidget::settingsChanged);
}

EraserSettingsWidget::~EraserSettingsWidget()
{
    delete ui;
}

int EraserSettingsWidget::radius() const
{
    return ui->radiusSlider->value();
}

void EraserSettingsWidget::on_radiusSlider_valueChanged(int value)
{
    ui->radiusValueLabel->setText(QString::number(value));
}

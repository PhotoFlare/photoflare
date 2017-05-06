#include "erasersettingswidget.h"
#include "ui_erasersettingswidget.h"

EraserSettingsWidget::EraserSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EraserSettingsWidget)
{
    ui->setupUi(this);
}

EraserSettingsWidget::~EraserSettingsWidget()
{
    delete ui;
}

int EraserSettingsWidget::brushWidth() const
{
    return ui->horizontalSlider->value();
}

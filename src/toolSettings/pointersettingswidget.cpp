#include "pointersettingswidget.h"
#include "ui_pointersettingswidget.h"

PointerSettingsWidget::PointerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointerSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->checkBoxStroke, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
    connect(ui->checkBoxFill, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));

    ui->checkBoxStroke->setDisabled(true);
    ui->checkBoxFill->setDisabled(true);
    ui->square_sel->setDisabled(true);
    ui->circle_sel->setVisible(false);
}

PointerSettingsWidget::~PointerSettingsWidget()
{
    delete ui;
}

bool PointerSettingsWidget::stroke() const
{
    return ui->checkBoxStroke->isChecked();
}

bool PointerSettingsWidget::fill() const
{
    return ui->checkBoxFill->isChecked();
}

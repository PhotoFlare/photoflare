/*

  Widget for paintbrushadv settings.

*/

#include "LineSettingsWidget.h"
#include "ui_LineSettingsWidget.h"
#include <QGraphicsPixmapItem>

LineSettingsWidget::LineSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineSettingsWidget)
{
    ui->setupUi(this);

    for(int i=1; i <= 6; i++)
    {
        QPixmap pixmap = QPixmap (QString(":/lines/lines/type_%1.png").arg(i));
        ui->styleComboBox->addItem(QString(), pixmap);
    }

    connect(ui->styleComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->opacitySpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->antialiasCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
}

LineSettingsWidget::~LineSettingsWidget()
{
    delete ui;
}

int LineSettingsWidget::width()
{
    return ui->widthSpinBox->value();
}

int LineSettingsWidget::opacity()
{
    return ui->opacitySpinBox->value();
}

bool LineSettingsWidget::antialias()
{
    return ui->antialiasCheckBox->isChecked();
}

int LineSettingsWidget::style()
{
    return ui->styleComboBox->currentIndex();
}

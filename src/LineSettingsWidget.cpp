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

    connect(ui->arrowCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
    connect(ui->arrowCheckBox2, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
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

int LineSettingsWidget::arrowStyle()
{
    if(ui->arrowCheckBox2->isChecked())
        return 2;
    else if(ui->arrowCheckBox->isChecked())
        return 1;
    else
        return 0;
}

void LineSettingsWidget::on_arrowCheckBox_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->arrowCheckBox2->setChecked(false);
}

void LineSettingsWidget::on_arrowCheckBox2_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->arrowCheckBox->setChecked(false);
}

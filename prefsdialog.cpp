#include "prefsdialog.h"
#include "ui_prefsdialog.h"
#include <QSettings>

prefsDialog::prefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prefsDialog)
{
    ui->setupUi(this);
    QSettings settings("Startup");
    bool maximize = settings.value("maximizeOnStartup").toBool();
    if(maximize==true) {
        ui->checkBoxMaximize->setChecked(true);
    }
    if(maximize==false) {
        ui->checkBoxManualSize->setChecked(true);
    }
    ui->spinBoxManualWidth->setValue(settings.value("manualWindowWidth").toInt());
    ui->spinBoxManualHeight->setValue(settings.value("manualWindowHeight").toInt());
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

void prefsDialog::on_buttonBox_accepted()
{
    QSettings settings("Startup");
    bool maximize = ui->checkBoxMaximize->isChecked();
    settings.setValue("maximizeOnStartup",maximize);
    settings.setValue("manualWindowWidth",ui->spinBoxManualWidth->value());
    settings.setValue("manualWindowHeight",ui->spinBoxManualHeight->value());
}

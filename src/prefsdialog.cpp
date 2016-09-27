/*

  Preferences dialog class.

*/

#include "prefsdialog.h"
#include "ui_prefsdialog.h"
#include "Settings.h"

prefsDialog::prefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prefsDialog)
{
    ui->setupUi(this);

    ui->checkBoxMaximize->setChecked(SETTINGS->isMaximizeWindow());
    ui->checkBox->setChecked(SETTINGS->isMultiWindowMode());

    QStringList list(QStringList() << "English" << "French" << "German" );
    ui->comboBoxLanguage->addItems(list);
    ui->comboBoxLanguage->setItemIcon(0,QIcon(":/pixmaps/pixmaps/en.png"));
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

void prefsDialog::on_buttonBox_accepted()
{
    if (ui->checkBoxMaximize->isChecked() != SETTINGS->isMaximizeWindow())
        SETTINGS->setMaximizeWindow(ui->checkBoxMaximize->isChecked());

    if (ui->checkBox->isChecked() != SETTINGS->isMultiWindowMode())
        SETTINGS->setMultiWindowMode(ui->checkBox->isChecked());
}

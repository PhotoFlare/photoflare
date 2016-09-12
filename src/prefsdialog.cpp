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
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

void prefsDialog::on_buttonBox_accepted()
{
    if (ui->checkBoxMaximize->isChecked() != SETTINGS->isMaximizeWindow())
        SETTINGS->setMaximizeWindow(ui->checkBoxMaximize->isChecked());
}

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

    QStringList list(QStringList() << "English" << "French" << "German" );
    ui->comboBoxLanguage->addItems(list);
    ui->comboBoxLanguage->setItemIcon(0,QIcon(":/pixmaps/flags/pixmaps/flags/United-kingdom.png"));
    ui->comboBoxLanguage->setItemIcon(1,QIcon(":/pixmaps/flags/pixmaps/flags/France.png"));
    ui->comboBoxLanguage->setItemIcon(2,QIcon(":/pixmaps/flags/pixmaps/flags/Germany.png"));

    ui->checkBoxMaximize->setChecked(SETTINGS->isMaximizeWindow());
    ui->checkBox->setChecked(SETTINGS->isMultiWindowMode());

    if(SETTINGS->getUserLanguage() == "en")
    ui->comboBoxLanguage->setCurrentIndex(0);

    if(SETTINGS->getUserLanguage() == "fr")
    ui->comboBoxLanguage->setCurrentIndex(1);

    if(SETTINGS->getUserLanguage() == "de")
    ui->comboBoxLanguage->setCurrentIndex(2);
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

    if(ui->comboBoxLanguage->currentIndex() == 0)
        SETTINGS->setUserLanguage("en");
    if(ui->comboBoxLanguage->currentIndex() == 1)
        SETTINGS->setUserLanguage("fr");
    if(ui->comboBoxLanguage->currentIndex() == 2)
        SETTINGS->setUserLanguage("de");
}

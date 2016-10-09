/*

  Preferences dialog class.

*/

#include "prefsdialog.h"
#include "ui_prefsdialog.h"
#include "Settings.h"
#include <QFileDialog>

prefsDialog::prefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prefsDialog)
{
    ui->setupUi(this);

    //Folders tab
    ui->openFolderLineEdit->setText(SETTINGS->getOpenFolder());
    ui->saveFolderLineEdit->setText(SETTINGS->getSaveFolder());
    ui->saveFormatEnabled->setChecked(SETTINGS->getSaveFormatEnabled());

    //Compression tab
    ui->compDialogEnabled->setChecked(SETTINGS->getCompressionDialogEnabled());
    ui->compEnabledDefault->setChecked(SETTINGS->getCompressionDefaultEnabled());
    ui->compressionValue->setText(SETTINGS->getCompressionDefaultValue());
    ui->compressionSlider->setValue(SETTINGS->getCompressionDefaultValue().toInt());

    //Saving tab
    QStringList filters;
    filters << tr("png (*.png)");
    filters << tr("jpg (*.jpg *.jpeg)");
    filters << tr("bmp (*.bmp)");
    filters << tr("pbm (*.pbm)");
    filters << tr("pgm (*.pgm)");
    filters << tr("ppm (*.ppm)");

    ui->comboBoxSaveFormat->addItems(filters);

    ui->comboBoxSaveFormat->setCurrentIndex(SETTINGS->getSaveFormat().toInt());

    //Startup tab
    //QStringList list(QStringList() << "English" << "French" << "German" );
    QStringList list(QStringList() << "English" << "French");
    ui->comboBoxLanguage->addItems(list);
    ui->comboBoxLanguage->setItemIcon(0,QIcon(":/pixmaps/flags/pixmaps/flags/United-kingdom.png"));
    ui->comboBoxLanguage->setItemIcon(1,QIcon(":/pixmaps/flags/pixmaps/flags/France.png"));
    //ui->comboBoxLanguage->setItemIcon(2,QIcon(":/pixmaps/flags/pixmaps/flags/Germany.png"));

    ui->checkBoxMaximize->setChecked(SETTINGS->isMaximizeWindow());
    ui->checkBox->setChecked(SETTINGS->isMultiWindowMode());

    if(SETTINGS->getUserLanguage() == "en")
        ui->comboBoxLanguage->setCurrentIndex(0);

    if(SETTINGS->getUserLanguage() == "fr")
        ui->comboBoxLanguage->setCurrentIndex(1);

    //if(SETTINGS->getUserLanguage() == "de")
    //ui->comboBoxLanguage->setCurrentIndex(2);
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

void prefsDialog::on_buttonBox_accepted()
{
    QString saveFormatEnabled = ui->saveFormatEnabled->isChecked() ? "true" : "false";
    QString compDefaultEnabled = ui->compEnabledDefault->isChecked() ? "true" : "false";
    QString compDialogEnabled = ui->compDialogEnabled->isChecked() ? "true" : "false";

    //Folders tab
    SETTINGS->setOpenFolder(ui->openFolderLineEdit->text());
    SETTINGS->setSaveFolder(ui->saveFolderLineEdit->text());
    SETTINGS->setSaveFormatEnabled(saveFormatEnabled);

    //Saving tab
    SETTINGS->setSaveFormat(QString::number(ui->comboBoxSaveFormat->currentIndex()));

    //Compression tab
    SETTINGS->setCompressionDialogEnabled(compDialogEnabled);
    SETTINGS->setCompressionDefaultEnabled(compDefaultEnabled);
    SETTINGS->setCompressionDefaultValue(ui->compressionValue->text());

    //Startup tab
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

void prefsDialog::on_openFolderButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->openFolderLineEdit->setText(dir);
}

void prefsDialog::on_saveFolderButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Save Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->saveFolderLineEdit->setText(dir);
}

void prefsDialog::on_compressionSlider_sliderMoved(int position)
{
    QString val = QString::number(position);
    ui->compressionValue->setText(val);
}

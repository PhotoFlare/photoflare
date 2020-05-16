/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// Preferences dialog class.

#include <QFileDialog>
#include <QProcess>
#include <QStandardPaths>

#include "prefsdialog.h"
#include "ui_prefsdialog.h"
#include "../Settings.h"

prefsDialog::prefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prefsDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    flagPath = ":/pixmaps/flags/assets/pixmaps/flags/";

    //Folders tab
    ui->openFolderLineEdit->setText(SETTINGS->getOpenFolder());
    ui->saveFolderLineEdit->setText(SETTINGS->getSaveFolder());
    ui->saveFormatEnabled->setChecked(SETTINGS->getSaveFormatEnabled());
    ui->previousOpenedEnabled->setChecked(SETTINGS->getPreviouslyOpened());
    ui->previousOpenedSaveEnabled->setChecked(SETTINGS->getPreviouslyOpenedSave());

    //Compression tab
    ui->compDialogEnabled->setChecked(SETTINGS->getCompressionDialogEnabled());
    ui->compEnabledDefault->setChecked(SETTINGS->getCompressionDefaultEnabled());
    ui->compressionValue->setText(SETTINGS->getCompressionDefaultValue());
    ui->compressionSlider->setValue(SETTINGS->getCompressionDefaultValue().toInt());

    //Layout tab
    QStringList toolList(QStringList() << tr("Right") << tr("Left"));
    ui->toolPaletteLocation->addItems(toolList);

    if(SETTINGS->getDockLayout() == "1")
    {
        ui->toolPaletteLocation->setCurrentIndex(1);
    }

    QStringList zoomList(QStringList() << tr("Default") << tr("Inverted"));
    ui->zoomDirection->addItems(zoomList);
    if(SETTINGS->getZoomDirection() == "1")
    {
        ui->zoomDirection->setCurrentIndex(1);
    }

    //Default values tab
    ui->memorizeParams->setChecked(SETTINGS->getMemParamsEnabled());
    ui->unitsCBox->setCurrentIndex(SETTINGS->getUnit());

    //Saving tab
    QStringList filters;
    filters << tr("png (*.png)");
    filters << tr("jpg (*.jpg *.jpeg)");
    filters << tr("bmp (*.bmp)");
    filters << tr("pbm (*.pbm)");
    filters << tr("pgm (*.pgm)");
    filters << tr("ppm (*.ppm)");
    filters << tr("ico (*.ico)");

    ui->comboBoxSaveFormat->addItems(filters);
    ui->comboBoxSaveFormat->setCurrentIndex(SETTINGS->getSaveFormat().toInt());

    //History tab
    ui->history_value->setText(SETTINGS->getHistoryLimit());
    ui->historySlider->setValue(SETTINGS->getHistoryLimit().toInt());

    //Startup tab
    QStringList list(QStringList() << tr("English") << tr("French") << tr("Dutch") << tr("German") << tr("Chinese (CN)") << tr("Chinese (TW)") << tr("Czech Republic") << tr("Portuguese (PT)") << tr("Portuguese (BR)") << tr("Sweden"));
    ui->comboBoxLanguage->addItems(list);
    ui->comboBoxLanguage->setItemIcon(0,QIcon(flagPath+"United-kingdom.png"));
    ui->comboBoxLanguage->setItemIcon(1,QIcon(flagPath+"France.png"));
    ui->comboBoxLanguage->setItemIcon(2,QIcon(flagPath+"Netherlands.png"));
    ui->comboBoxLanguage->setItemIcon(3,QIcon(flagPath+"Germany.png"));
    ui->comboBoxLanguage->setItemIcon(4,QIcon(flagPath+"China.png"));
    ui->comboBoxLanguage->setItemIcon(5,QIcon(flagPath+"Taiwan.png"));
    ui->comboBoxLanguage->setItemIcon(6,QIcon(flagPath+"Czech-Republic.png"));
    ui->comboBoxLanguage->setItemIcon(7,QIcon(flagPath+"Portugal.png"));
    ui->comboBoxLanguage->setItemIcon(8,QIcon(flagPath+"Brazil.png"));
    ui->comboBoxLanguage->setItemIcon(9,QIcon(flagPath+"Sweden.png"));

    ui->checkBoxMaximize->setChecked(SETTINGS->isMaximizeWindow());
    ui->checkBox->setChecked(SETTINGS->isMultiWindowMode());

    if(SETTINGS->getUserLanguage() == "en")
        ui->comboBoxLanguage->setCurrentIndex(0);
    else if(SETTINGS->getUserLanguage() == "fr")
        ui->comboBoxLanguage->setCurrentIndex(1);
    else if(SETTINGS->getUserLanguage() == "nl")
        ui->comboBoxLanguage->setCurrentIndex(2);
    else if(SETTINGS->getUserLanguage() == "de")
        ui->comboBoxLanguage->setCurrentIndex(3);
    else if(SETTINGS->getUserLanguage() == "zh_CN")
        ui->comboBoxLanguage->setCurrentIndex(4);
    else if(SETTINGS->getUserLanguage() == "zh_TW")
        ui->comboBoxLanguage->setCurrentIndex(5);
    else if(SETTINGS->getUserLanguage() == "cs")
        ui->comboBoxLanguage->setCurrentIndex(6);
    else if(SETTINGS->getUserLanguage() == "pt_PT")
        ui->comboBoxLanguage->setCurrentIndex(7);
    else if(SETTINGS->getUserLanguage() == "pt_BR")
        ui->comboBoxLanguage->setCurrentIndex(8);
    else if(SETTINGS->getUserLanguage() == "sv")
        ui->comboBoxLanguage->setCurrentIndex(9);

    ui->restartButton->hide();
}

prefsDialog::~prefsDialog()
{
    delete ui;
}

void prefsDialog::on_buttonBox_accepted()
{
    //Folders tab
    SETTINGS->setOpenFolder(ui->openFolderLineEdit->text());
    SETTINGS->setSaveFolder(ui->saveFolderLineEdit->text());
    SETTINGS->setSaveFormatEnabled(ui->saveFormatEnabled->isChecked());
    SETTINGS->setPreviouslyOpened(ui->previousOpenedEnabled->isChecked());
    SETTINGS->setPreviouslyOpenedSave(ui->previousOpenedSaveEnabled->isChecked());

    //Layout tab
    SETTINGS->setDockLayout(QString::number(ui->toolPaletteLocation->currentIndex()));
    SETTINGS->setZoomDirection(QString::number(ui->zoomDirection->currentIndex()));

    //Default values tab
    SETTINGS->setMemParamsEnabled(ui->memorizeParams->isChecked());
    SETTINGS->setUnit(ui->unitsCBox->currentIndex());

    //Saving tab
    SETTINGS->setSaveFormat(QString::number(ui->comboBoxSaveFormat->currentIndex()));

    //History tab
    SETTINGS->setHistoryLimit(ui->history_value->text());

    //Compression tab
    SETTINGS->setCompressionDialogEnabled(ui->compDialogEnabled->isChecked());
    SETTINGS->setCompressionDefaultEnabled(ui->compEnabledDefault->isChecked());
    SETTINGS->setCompressionDefaultValue(ui->compressionValue->text());

    //Startup tab
    if (ui->checkBoxMaximize->isChecked() != SETTINGS->isMaximizeWindow())
        SETTINGS->setMaximizeWindow(ui->checkBoxMaximize->isChecked());

    if (ui->checkBox->isChecked() != SETTINGS->isMultiWindowMode())
        SETTINGS->setMultiWindowMode(ui->checkBox->isChecked());

    // Save language when we close the dialog
    set_user_language();
}

void prefsDialog::on_buttonBox_rejected()
{
    // Setup Default settings
    QString loc = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory)+"photoflare";
    if(QDir(loc).exists() == false)
    {
        SETTINGS->setDefaultSettings();
    }
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

void prefsDialog::on_compressionSlider_valueChanged(int value)
{
    QString val = QString::number(value);
    ui->compressionValue->setText(val);
}

void prefsDialog::on_historySlider_sliderMoved(int position)
{
    QString val = QString::number(position);
    ui->history_value->setText(val);
}

void prefsDialog::on_historySlider_valueChanged(int value)
{
    QString val = QString::number(value);
    ui->history_value->setText(val);
}

void prefsDialog::on_restartButton_clicked()
{
    // Save language before we quit
    set_user_language();

    qApp->quit();
    QProcess::startDetached(QCoreApplication::applicationFilePath());
}

void prefsDialog::on_comboBoxLanguage_currentIndexChanged()
{
    ui->restartButton->show();
}

void prefsDialog::set_user_language()
{
    if(ui->comboBoxLanguage->currentIndex() == 0)
        SETTINGS->setUserLanguage("en");
    else if(ui->comboBoxLanguage->currentIndex() == 1)
        SETTINGS->setUserLanguage("fr");
    else if(ui->comboBoxLanguage->currentIndex() == 2)
        SETTINGS->setUserLanguage("nl");
    else if(ui->comboBoxLanguage->currentIndex() == 3)
        SETTINGS->setUserLanguage("de");
    else if(ui->comboBoxLanguage->currentIndex() == 4)
        SETTINGS->setUserLanguage("zh_CN");
    else if(ui->comboBoxLanguage->currentIndex() == 5)
        SETTINGS->setUserLanguage("zh_TW");
    else if(ui->comboBoxLanguage->currentIndex() == 6)
        SETTINGS->setUserLanguage("cs");
    else if(ui->comboBoxLanguage->currentIndex() == 7)
        SETTINGS->setUserLanguage("pt_PT");
    else if(ui->comboBoxLanguage->currentIndex() == 8)
        SETTINGS->setUserLanguage("pt_BR");
    else if(ui->comboBoxLanguage->currentIndex() == 9)
        SETTINGS->setUserLanguage("sv");
}

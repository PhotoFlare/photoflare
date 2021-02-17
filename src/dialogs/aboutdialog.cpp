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

// About dialog class for the Photoflare application.

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->creditsWidget->setVisible(false);

    ui->devTextArea->setText(tr(
        "Commercial - ICS"
        "<br>Lead - Dylan Coakley"
        "<br>Developer - Marek Mosna"
        "<br>Developer - Dmitry Vasilenko"
        "<br>Debian package - Dylan Aïssi"
        "<br>AppImage package - probonopd")
    );
    ui->graphicsTextArea->setText(tr(
        "Main icon - Robert BM"
        "<br>Website graphics - Raqasa"
        "<br>Application icons - Attari B"
        "<br>Art Designer - Catherine Hegedusich"
        "<br>Advertising - Don Murphy")
    );

    ui->translateTextArea->setText(tr(
        "FR - Ludovic, N."
        "<br>NL - Freek P."
        "<br>DE - Steffen G."
        "<br>zh_TW - Coolshou"
        "<br>zh_CN - Nichts H."
        "<br>CS - Pafri"
        "<br>pt_BR - eltonfabricio10"
        "<br>pt_PT - eltonfabricio10"
        "<br>SV - eson57"
        "<br>RU - Alex K."
        "<br>ID - triyanwn"
        "<br>ES - darkshram")
    );
    ui->contribTextArea->setText(tr(
        "eltonfabricio10 - Code"
        "<br>AngryPenguinPL - Code"
        "<br>Carlo Vaccari - Code"
        "<br>Duzy Chan - Code"
        "<br>Samuel Cowen - Code"
        "<br>Richard Senior - GNU Autotools"
        "<br>Alexandre Kharlamov - Code"
        "<br>Michael Sheppard - Code"
        "<br>Helene Levernieux - Project Icon"
        "<br><br><b>Special thanks"
        "</b><br>I would like to thank Samantha for being an absolute gem of a person."
        " You light up my life and I'm so glad to have found you.")
    );

    QString appName = qApp->applicationName().replace("p","P");
    ui->appName->setText(appName);
    QFont font;
    font.setPointSize(14);
    ui->appName->setFont(font);
    ui->appVersion->setText(qApp->applicationVersion()+tr(" Community Edition"));
    ui->appDescription->setText(tr("A simple but featureful editor."));
    ui->appUrl->setText(tr("<a href='https://www.photoflare.io'>https://www.photoflare.io</a>"));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_closeButton_clicked()
{
    close();
}

void aboutDialog::on_creditsButton_clicked()
{
    if (ui->creditsWidget->isVisible()==false)
    {
        ui->creditsWidget->setVisible(true);
    }
    else if (ui->creditsWidget->isVisible()==true)
    {
        ui->creditsWidget->setVisible(false);
    }

}

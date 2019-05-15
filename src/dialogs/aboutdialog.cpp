/*

  About dialog class for the PhotoFlare application.

*/

#include <QApplication>
#include <QTextEdit>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->creditsTextArea->setVisible(false);
    ui->creditsTextArea->setText(
                tr("<b>Developers</b>"
                "<br>Commercial - ICS"
                "<br>Lead - Dylan Coakley"
                "<br>Developer - Marek Mosna"
                "<br>Developer - Dmitry Vasilenko"
                "<br><br><b>Packaging</b>"
                "<br>Debian - Dylan Aissi"
                "<br>AppImage - probonopd"
                "<br><br><b>Graphics</b>"
                "<br>Main icon - Robert BM"
                "<br>Website graphics - Raqasa"
                "<br>Application icons - Attari B"
                "<br><br><b>Promotional Media</b>"
                "<br>Art Designer - Catherine Fister"
                "<br>Advertising - Don Murphy"
                "<br><br><b>Contributors</b>"
                "<br>Duzy Chan - Code"
                "<br>Samuel Cowen - Code"
                "<br>Richard Senior - GNU Autotools"
                "<br>Alexandre Kharlamov - Code"
                "<br>Michael Sheppard - Code"
                "<br>Helene Levernieux - Project Icon"
                "<br><br><b>Translators</b>"
                "<br>FR - Ludovic, Nerdouille"
                "<br>NL - Freek Pol"
                "<br>DE - Freacore"
                "<br>zh_TW - Coolshou"
                "<br><br><b>Special thanks"
                "</b><br>I would like to thank Samantha for being an absolute gem of a person. You light up my life and I'm so glad to have found you."));

    QString appName = qApp->applicationName().replace("p","P");
    ui->appName->setText(appName);
    QFont font;
    font.setPointSize(14);
    ui->appName->setFont(font);
    ui->appVersion->setText(qApp->applicationVersion()+tr(" Community Edition"));
    ui->appDescription->setText(tr("A simple but featureful editor."));
    ui->appUrl->setText(tr("<a href='http://www.photoflare.io'>http://www.photoflare.io</a>"));
    ui->appName->setAlignment(Qt::AlignCenter);
    ui->appVersion->setAlignment(Qt::AlignCenter);
    ui->appDescription->setAlignment(Qt::AlignCenter);
    ui->appUrl->setAlignment(Qt::AlignCenter);
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::on_pushButton_clicked()
{
    close();
}

void aboutDialog::on_pushButton_2_clicked()
{
    if (ui->creditsTextArea->isVisible()==false)
    {
        ui->creditsTextArea->setVisible(true);
        creditsVisible=true;
    }
    else if (ui->creditsTextArea->isVisible()==true)
    {
        ui->creditsTextArea->setVisible(false);
        creditsVisible=false;
    }

}

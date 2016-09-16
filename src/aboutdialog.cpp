/*

  About dialog class for the PhotoFiltre LX application.

*/

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QApplication>
#include <QTextEdit>

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    bool creditsVisible = false;
    ui->setupUi(this);
    ui->creditsTextArea->setVisible(creditsVisible);
    ui->creditsTextArea->setText(tr("<b>Developers</b><br>Commercial - ICS<br>Lead - Dylan Coakley<br>Developer - Ackios<br><br><b>Graphics</b><br>Website graphics - Raqasa<br>Application icons - Attari B<br><br><b>Promotional Media</b><br>Art Designer - Catherine Fister<br>Advertising - Don Murphy<br><br><b>Contributors</b><br>Duzy Chan - Code<br>Samuel Cowen - Code<br>Richard Senior - GNU Autotools<br>Alexandre Kharlamov - Code<br>Michael Sheppard - Code<br>Helene Levernieux - Project Icon<br><br><b>Special thanks</b><br>I would to thank Catherine for her strong support of myself and the project over the years. You gave me the courage to keep going and accomplish my goals."));

    ui->appName->setText(qApp->applicationName());
    QFont font;
    font.setPointSize(14);
    ui->appName->setFont(font);
    ui->appVersion->setText(qApp->applicationVersion());
    ui->appDescription->setText(tr("A simple but featureful editor."));
    ui->appUrl->setText(tr("<a href='http://www.photofiltre-lx.org'>http://www.photofiltre-lx.org</a>"));
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
    if (creditsVisible==false) {
        ui->creditsTextArea->setVisible(true);
        creditsVisible=true;
    }
    else if (creditsVisible==true) {
        ui->creditsTextArea->setVisible(false);
        creditsVisible=false;
    }

}

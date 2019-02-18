/*

  Dialog for checking version updates.

*/

//#include <QDebug>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVersionNumber>
#include <QDesktopServices>

#include "checkupdatedialog.h"
#include "ui_checkupdatedialog.h"

checkupdateDialog::checkupdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkupdateDialog)
{
    ui->setupUi(this);
    ui->downloadButton->setVisible(false);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://raw.githubusercontent.com/PhotoFlare/photoflare/master/versions/stable.json")));
}

checkupdateDialog::~checkupdateDialog()
{
    delete ui;
}

void checkupdateDialog::replyFinished(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();
    QString currentversion = QString::fromUtf8(data);

    QVersionNumber ver1 = QVersionNumber::fromString(currentversion);
    QVersionNumber ver2 = QVersionNumber::fromString(qApp->applicationVersion());

    int result = QVersionNumber::compare(ver1,ver2);

    if(result > 0)
    {
        ui->label->setText(tr("New version available"));
        ui->downloadButton->setVisible(true);
    }
    else
    {
        ui->label->setText(tr("No updates available"));
    }

    emit finished();
}

void checkupdateDialog::on_closeButton_clicked()
{
    close();
}

void checkupdateDialog::on_downloadButton_clicked()
{
    QString link = "https://photoflare.io/downloads/";
    QDesktopServices::openUrl(QUrl(link));
}

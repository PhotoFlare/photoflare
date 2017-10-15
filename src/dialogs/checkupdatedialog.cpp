#include <QDebug>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "checkupdatedialog.h"
#include "ui_checkupdatedialog.h"

checkupdateDialog::checkupdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkupdateDialog)
{
    ui->setupUi(this);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("https://raw.githubusercontent.com/PhotoFlare/photoflare/master/versions/dev.json")));
}

checkupdateDialog::~checkupdateDialog()
{
    delete ui;
}

void checkupdateDialog::replyFinished(QNetworkReply* reply)
{
    qDebug() << reply->readAll();
    qDebug() << qApp->applicationVersion();

    emit finished();
}

void checkupdateDialog::on_closeButton_clicked()
{
    close();
}

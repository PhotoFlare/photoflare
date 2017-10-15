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

    QString data = "";

    manager->get(QNetworkRequest(QUrl("http://photoflare.io/version/stable.json")));
}

checkupdateDialog::~checkupdateDialog()
{
    delete ui;
}



void checkupdateDialog::replyFinished(QNetworkReply* reply)
{
    qDebug() << reply->readAll();

    emit finished();
}

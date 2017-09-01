#include "plugindialog.h"
#include "ui_plugindialog.h"

#include <QDebug>
#include <QDir>

PluginDialog::PluginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    QDir dir("plugins");
    if (!dir.exists())
    {
        qWarning("Cannot find the plugins directory");
    }
    else
    {
        ui->listWidget->addItems(dir.entryList(QDir::NoDotAndDotDot|QDir::AllEntries));
    }

    int count = ui->listWidget->count();
    for(int i = 0; i < count; i++)
    {
      QListWidgetItem *item = ui->listWidget->item(i);
      item->setSizeHint(QSize(item->sizeHint().width(), 50));
    }
}

PluginDialog::~PluginDialog()
{
    delete ui;
}

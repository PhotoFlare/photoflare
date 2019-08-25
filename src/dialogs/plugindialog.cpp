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

#include "plugindialog.h"
#include "ui_plugindialog.h"

//#include <QDebug>
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
        //qWarning("Cannot find the plugins directory");
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

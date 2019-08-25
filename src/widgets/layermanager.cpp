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

#include "layermanager.h"
#include "ui_layermanager.h"

LayerManager::LayerManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayerManager)
{
    ui->setupUi(this);
}

LayerManager::~LayerManager()
{
    delete ui;
}

void LayerManager::addLayer()
{
    layercount = getLayerCount()+1;

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(QString("Layer "+QString::number(layercount)));
    item->setFlags(item->flags () | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    ui->listWidget->addItem(item);
}

void LayerManager::deleteLayer()
{
    qDeleteAll(ui->listWidget->selectedItems());
}

int LayerManager::getLayerCount()
{
    return ui->listWidget->count();
}

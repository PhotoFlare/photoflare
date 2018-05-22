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

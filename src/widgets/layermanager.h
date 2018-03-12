#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>

namespace Ui {
class LayerManager;
}

class LayerManager : public QWidget
{
    Q_OBJECT

public:
    explicit LayerManager(QWidget *parent = 0);
    ~LayerManager();
    void addLayer();
    void deleteLayer();
    int getLayerCount();

private:
    Ui::LayerManager *ui;
    int layercount = 1;
};

#endif // LAYERMANAGER_H

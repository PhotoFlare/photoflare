#include "filterworker.h"
#include "FilterManager.h"
#include "PaintWidget.h"
#include <QDebug>

FilterWorker::FilterWorker(QObject *parent) : QObject(parent)
{

}

void FilterWorker::setImage(QImage image)
{
    currentImage = image;
}

void FilterWorker::process()
{
    qDebug() << "test";
     QImage newImage;
    //newImage = FilterManager::instance()->wave(currentImage);

    emit fileProcessFinished(newImage);
}

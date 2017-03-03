#include "filterworker.h"
#include "FilterManager.h"
#include "PaintWidget.h"
#include <QDebug>

FilterWorker::FilterWorker(QObject *parent) : QObject(parent)
{

}

void FilterWorker::setFilter(QString filter)
{
	currentFilter = filter;
}

void FilterWorker::setImage(QImage image)
{
    currentImage = image;
}

void FilterWorker::process()
{
    qDebug() << "test";
     QImage newImage;

     if(currentFilter == "wave")
     {
    	newImage = FilterManager::instance()->wave(currentImage);
     }

    emit fileProcessFinished(newImage);
}

#include "filterworker.h"
#include "FilterManager.h"
#include "PaintWidget.h"
//#include <QDebug>

FilterWorker::FilterWorker(QObject *parent) : QObject(parent)
{

}

void FilterWorker::setImage(QImage image)
{
    currentImage = image;
}

void FilterWorker::setFilter(QString filter)
{
    currentFilter = filter;
}

void FilterWorker::process()
{
    QImage newImage;

    if(currentFilter == "wave")
    {
        newImage = FilterManager::instance()->wave(currentImage);
    }
    else if(currentFilter == "swirl")
    {
        newImage = FilterManager::instance()->swirl(currentImage);
    }
    else if(currentFilter == "reinforce")
    {
        newImage = FilterManager::instance()->reinforce(currentImage);
    }
    else if(currentFilter == "dustreduction")
    {
        newImage = FilterManager::instance()->dustreduction(currentImage);
    }
    else if(currentFilter == "deSpeckle")
    {
        newImage = FilterManager::instance()->deSpeckle(currentImage);
    }
    else if(currentFilter == "gaussianNoise")
    {
        newImage = FilterManager::instance()->gaussianNoise(currentImage);
    }
    else if(currentFilter == "impulseNoise")
    {
        newImage = FilterManager::instance()->impulseNoise(currentImage);
    }
    else if(currentFilter == "laplacianNoise")
    {
        newImage = FilterManager::instance()->laplacianNoise(currentImage);
    }
    else if(currentFilter == "poissonNoise")
    {
        newImage = FilterManager::instance()->poissonNoise(currentImage);
    }
    else if(currentFilter == "grayscale")
    {
        newImage = FilterManager::instance()->grayscale(currentImage);
    }
    else if(currentFilter == "oldPhoto")
    {
        newImage = FilterManager::instance()->oldPhoto(currentImage);
    }
    else if(currentFilter == "sepia")
    {
        newImage = FilterManager::instance()->sepia(currentImage);
    }
    else if(currentFilter == "equalizeColours")
    {
        newImage = FilterManager::instance()->equalizeColours(currentImage);
    }
    else if(currentFilter == "normalize")
    {
        newImage = FilterManager::instance()->normalize(currentImage);
    }
    else if(currentFilter == "blackwhite")
    {
        newImage = FilterManager::instance()->blackwhite(currentImage);
    }
    else if(currentFilter == "oil")
    {
        newImage = FilterManager::instance()->oilPaint(currentImage);
    }
    else if(currentFilter == "charcoal")
    {
        newImage = FilterManager::instance()->charcoal(currentImage);
    }
    else if(currentFilter == "solarize")
    {
        newImage = FilterManager::instance()->solarize(currentImage);
    }
    else if(currentFilter == "motionBlur")
    {
        newImage = FilterManager::instance()->motionBlur(currentImage);
    }
    else if(currentFilter == "implode")
    {
        newImage = FilterManager::instance()->implode(currentImage);
    }
    else if(currentFilter == "explode")
    {
        newImage = FilterManager::instance()->explode(currentImage);
    }
    else if(currentFilter == "cropToCenter")
    {
        newImage = FilterManager::instance()->cropToCenter(currentImage);
    }
    else if(currentFilter == "simpleFrame")
    {
        newImage = FilterManager::instance()->simpleFrame(currentImage);
    }
    else if(currentFilter == "advFrame")
    {
        newImage = FilterManager::instance()->advFrame(currentImage);
    }
    else if(currentFilter == "emboss")
    {
        newImage = FilterManager::instance()->emboss(currentImage);
    }
    else if(currentFilter == "monoChromeEdges")
    {
        newImage = FilterManager::instance()->monoChromeEdges(currentImage);
    }
    emit filterProcessFinished(newImage);


}

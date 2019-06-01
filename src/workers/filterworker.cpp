#include "filterworker.h"
#include "FilterManager.h"
#include "PaintWidget.h"
#include <vector>
#include <cmath>
#include "omp.h"

#include <QDebug>

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

void FilterWorker::setDoubleVal(double v)
{
    currentDouble = v;
}

void FilterWorker::process()
{
    QImage newImage = currentImage;

    int maxThreads = omp_get_max_threads();
    int w = currentImage.width();
    int h = currentImage.height();

    #pragma omp parallel shared(currentImage, newImage) firstprivate(maxThreads, w, h)
{
    #pragma omp for schedule(dynamic) nowait
    for(int thread=0;thread<maxThreads;thread++) {
        int top = std::ceil(h/float(maxThreads))*thread;
        int bottom = std::min(h,int(std::ceil(h/float(maxThreads))*(thread+1)))-1;

//        qDebug() << "iteration: " << thread;
        QImage currentImageSlice = currentImage.copy(0,top,w,bottom-top);
//        qDebug() << "args: " << 0 << ", " << top << ", " << w << ", " << bottom;
        QImage newImageSlice(currentImageSlice);
//        qDebug() << "current image dims: " +currentImageSlice.height() << ", " << currentImageSlice.width();

        if(currentFilter == "gammacorrectminus")
        {
            newImageSlice = FilterManager::instance()->setGamma(currentImageSlice, 0.9);
        }
        else if(currentFilter == "gammacorrectplus")
        {
            newImageSlice = FilterManager::instance()->setGamma(currentImageSlice, 1.1);
        }
        else if(currentFilter == "saturationminus")
        {
            newImageSlice = FilterManager::instance()->setSaturation(currentImageSlice, -15);
        }
        else if(currentFilter == "saturationplus")
        {
            newImageSlice = FilterManager::instance()->setSaturation(currentImageSlice, +15);
        }
        else if(currentFilter == "contrastminus")
        {
            newImageSlice = FilterManager::instance()->setContrast(currentImageSlice, 0.5);
        }
        else if(currentFilter == "contrastplus")
        {
            newImageSlice = FilterManager::instance()->setContrast(currentImageSlice, 1.0);
        }
        else if(currentFilter == "brightminus")
        {
            newImageSlice = FilterManager::instance()->setBrightness(currentImageSlice, -5);
        }
        else if(currentFilter == "brightplus")
        {
            newImageSlice = FilterManager::instance()->setBrightness(currentImageSlice, 5);
        }
        else if(currentFilter == "flipHorz")
        {
            newImageSlice = FilterManager::instance()->flipHorz(currentImageSlice);
        }
        else if(currentFilter == "flipVert")
        {
            newImageSlice = FilterManager::instance()->flipVert(currentImageSlice);
        }
        else if(currentFilter == "rotateCCW")
        {
            newImageSlice = FilterManager::instance()->rotateCCW(currentImageSlice);
        }
        else if(currentFilter == "rotateCW")
        {
            newImageSlice = FilterManager::instance()->rotateCW(currentImageSlice);
        }
        else if(currentFilter == "trim")
        {
            newImageSlice = FilterManager::instance()->trim(currentImageSlice);
        }
        else if(currentFilter == "dropShadow")
        {
            newImageSlice = FilterManager::instance()->dropShadow(currentImageSlice, 5, 40, Qt::black,0,10);
        }
        else if(currentFilter == "autoLevels")
        {
            newImageSlice = FilterManager::instance()->autoLevels(currentImageSlice);
        }
        else if(currentFilter == "autoContrast")
        {
            newImageSlice = FilterManager::instance()->autoContrast(currentImageSlice);
        }
        else if(currentFilter == "negative")
        {
            newImageSlice = FilterManager::instance()->negative(currentImageSlice);
        }
        else if(currentFilter == "setOpacity")
        {
            newImageSlice = FilterManager::instance()->setOpacity(currentImageSlice,currentDouble);
        }
        else if(currentFilter == "soften")
        {
            newImageSlice = FilterManager::instance()->soften(currentImageSlice);
        }
        else if(currentFilter == "blur")
        {
            newImageSlice = FilterManager::instance()->blur(currentImageSlice);
        }
        else if(currentFilter == "sharpen")
        {
            newImageSlice = FilterManager::instance()->sharpen(currentImageSlice);
        }
        else if(currentFilter == "reinforce")
        {
            newImageSlice = FilterManager::instance()->reinforce(currentImageSlice);
        }
        else if(currentFilter == "dustreduction")
        {
            newImageSlice = FilterManager::instance()->dustreduction(currentImageSlice);
        }
        else if(currentFilter == "deSpeckle")
        {
            newImageSlice = FilterManager::instance()->deSpeckle(currentImageSlice);
        }
        else if(currentFilter == "gaussianNoise")
        {
            newImageSlice = FilterManager::instance()->gaussianNoise(currentImageSlice);
        }
        else if(currentFilter == "impulseNoise")
        {
            newImageSlice = FilterManager::instance()->impulseNoise(currentImageSlice);
        }
        else if(currentFilter == "laplacianNoise")
        {
            newImageSlice = FilterManager::instance()->laplacianNoise(currentImageSlice);
        }
        else if(currentFilter == "poissonNoise")
        {
            newImageSlice = FilterManager::instance()->poissonNoise(currentImageSlice);
        }
        else if(currentFilter == "grayscale")
        {
            newImageSlice = FilterManager::instance()->grayscale(currentImageSlice);
        }
        else if(currentFilter == "oldPhoto")
        {
            newImageSlice = FilterManager::instance()->oldPhoto(currentImageSlice);
        }
        else if(currentFilter == "sepia")
        {
            newImageSlice = FilterManager::instance()->sepia(currentImageSlice);
        }
        else if(currentFilter == "equalizeColours")
        {
            newImageSlice = FilterManager::instance()->equalizeColours(currentImageSlice);
        }
        else if(currentFilter == "normalize")
        {
            newImageSlice = FilterManager::instance()->normalize(currentImageSlice);
        }
        else if(currentFilter == "blackwhite")
        {
            newImageSlice = FilterManager::instance()->blackwhite(currentImageSlice);
        }
        else if(currentFilter == "oil")
        {
            newImageSlice = FilterManager::instance()->oilPaint(currentImageSlice);
        }
        else if(currentFilter == "charcoal")
        {
            newImageSlice = FilterManager::instance()->charcoal(currentImageSlice);
        }
        else if(currentFilter == "solarize")
        {
            newImageSlice = FilterManager::instance()->solarize(currentImageSlice);
        }
        else if(currentFilter == "motionBlur")
        {
            newImageSlice = FilterManager::instance()->motionBlur(currentImageSlice);
        }
        else if(currentFilter == "swirl")
        {
            newImageSlice = FilterManager::instance()->swirl(currentImageSlice);
        }
        else if(currentFilter == "wave")
        {
            newImageSlice = FilterManager::instance()->wave(currentImageSlice);
        }
        else if(currentFilter == "implode")
        {
            newImageSlice = FilterManager::instance()->implode(currentImageSlice);
        }
        else if(currentFilter == "explode")
        {
            newImageSlice = FilterManager::instance()->explode(currentImageSlice);
        }
        else if(currentFilter == "cropToCenter")
        {
            newImageSlice = FilterManager::instance()->cropToCenter(currentImageSlice);
        }
        else if(currentFilter == "simpleFrame")
        {
            newImageSlice = FilterManager::instance()->simpleFrame(currentImageSlice);
        }
        else if(currentFilter == "advFrame")
        {
            newImageSlice = FilterManager::instance()->advFrame(currentImageSlice);
        }
        else if(currentFilter == "emboss")
        {
            newImageSlice = FilterManager::instance()->emboss(currentImageSlice);
        }
        else if(currentFilter == "monoChromeEdges")
        {
            newImageSlice = FilterManager::instance()->monoChromeEdges(currentImageSlice);
        }
        else if(currentFilter == "colourthreshold")
        {
            newImageSlice = FilterManager::instance()->colourthreshold(currentImageSlice);
        }


        for(int y=0; y<bottom-top;y++)
        {
            int outputY = y+top;
            for(int x=0;x<w;x++)
            {
                newImage.setPixel(x,outputY,newImageSlice.pixel(x,y));
            }
        }
    }
}

    emit filterProcessFinished(newImage);
}

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

// Worker class for the application to apply the filters. (Single threaded - Not all filters work with OpenMP)

#include "filterworker.h"
#include "FilterManager.h"

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

void FilterWorker::setDoubleVal(double v)
{
    currentDouble = v;
}

void FilterWorker::process()
{
    QImage newImage;

    if(currentFilter == "gammacorrectminus")
    {
        newImage = FilterManager::instance()->setGamma(currentImage, 0.9);
    }
    else if(currentFilter == "gammacorrectplus")
    {
        newImage = FilterManager::instance()->setGamma(currentImage, 1.1);
    }
    else if(currentFilter == "saturationminus")
    {
        newImage = FilterManager::instance()->setSaturation(currentImage, -15);
    }
    else if(currentFilter == "saturationplus")
    {
        newImage = FilterManager::instance()->setSaturation(currentImage, +15);
    }
    else if(currentFilter == "contrastminus")
    {
        newImage = FilterManager::instance()->setContrast(currentImage, 0.5);
    }
    else if(currentFilter == "contrastplus")
    {
        newImage = FilterManager::instance()->setContrast(currentImage, 1.0);
    }
    else if(currentFilter == "brightminus")
    {
        newImage = FilterManager::instance()->setBrightness(currentImage, -5);
    }
    else if(currentFilter == "brightplus")
    {
        newImage = FilterManager::instance()->setBrightness(currentImage, 5);
    }
    else if(currentFilter == "flipHorz")
    {
        newImage = FilterManager::instance()->flipHorz(currentImage);
    }
    else if(currentFilter == "flipVert")
    {
        newImage = FilterManager::instance()->flipVert(currentImage);
    }
    else if(currentFilter == "rotateCCW")
    {
        newImage = FilterManager::instance()->rotateCCW(currentImage);
    }
    else if(currentFilter == "rotateCW")
    {
        newImage = FilterManager::instance()->rotateCW(currentImage);
    }
    else if(currentFilter == "rotate")
    {
        newImage = FilterManager::instance()->rotate(currentImage, currentDouble);
    }
    else if(currentFilter == "trim")
    {
        newImage = FilterManager::instance()->trim(currentImage);
    }
    else if(currentFilter == "dropShadow")
    {
        newImage = FilterManager::instance()->dropShadow(currentImage, 5, 40, Qt::black,0,10);
    }
    else if(currentFilter == "autoLevels")
    {
        newImage = FilterManager::instance()->autoLevels(currentImage);
    }
    else if(currentFilter == "autoContrast")
    {
        newImage = FilterManager::instance()->autoContrast(currentImage);
    }
    else if(currentFilter == "negative")
    {
        newImage = FilterManager::instance()->negative(currentImage);
    }
    else if(currentFilter == "setOpacity")
    {
        newImage = FilterManager::instance()->setOpacity(currentImage,currentDouble);
    }
    else if(currentFilter == "soften")
    {
        newImage = FilterManager::instance()->soften(currentImage);
    }
    else if(currentFilter == "blur")
    {
        newImage = FilterManager::instance()->blur(currentImage);
    }
    else if(currentFilter == "sharpen")
    {
        newImage = FilterManager::instance()->sharpen(currentImage);
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
    else if(currentFilter == "swirl")
    {
        newImage = FilterManager::instance()->swirl(currentImage);
    }
    else if(currentFilter == "wave")
    {
        newImage = FilterManager::instance()->wave(currentImage);
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
    else if(currentFilter == "colourthreshold")
    {
        newImage = FilterManager::instance()->colourthreshold(currentImage);
    }
    emit filterProcessFinished(newImage);
}

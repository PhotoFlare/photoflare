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

// Worker class for the BatchDialog to process the files.

//#include <QDebug>
#include <QFileInfo>

#include "BatchProcessWorker.h"
#include "PaintWidget.h"
#include "FilterManager.h"

BatchProcessWorker::BatchProcessWorker(QObject *parent) : QObject(parent), m_cancelled(false)
{
}

void BatchProcessWorker::cancel()
{
    m_cancelled = true;
}

void BatchProcessWorker:: process()
{
    int i = 0;
    foreach (QString file, m_params->fileList()) {
        if (m_cancelled)
            break;
        QImage image(file);
        if(m_params->changeImageSize())
        {
            if(!m_params->imageSizeUnits())
            {
                image = image.scaled(m_params->imageSize());
            } else {
                image = image.scaled( QSize(
                       image.width() * m_params->imageSize().width() / 100,
                       image.height() * m_params->imageSize().height() / 100));
            }
        }

        if(m_params->changeCanvasSize())
        {
            QImage canvas (m_params->canvasSize(), QImage::Format_ARGB32_Premultiplied);
            canvas.fill(m_params->backgroundColor());
            QPoint pos;
            QPainter painter(&canvas);
            switch(m_params->imagePosition())
            {
                case LeftTop:
                    pos = QPoint(0,0);
                    break;
                case CenterTop:
                    pos = QPoint(canvas.width()/2 - image.width()/2, 0);
                    break;
                case RightTop:
                    pos = QPoint(canvas.width() - image.width(), 0);
                    break;
                case LeftCenter:
                    pos = QPoint(0, canvas.height()/2 - image.height()/2);
                    break;
                case CenterCenter:
                    pos = QPoint(canvas.width()/2 - image.width()/2, canvas.height()/2 - image.height()/2);
                    break;
                case RightCenter:
                    pos = QPoint(canvas.width() - image.width(), canvas.height()/2 - image.height()/2);
                    break;
                case LeftBottom:
                    pos = QPoint(0, canvas.height() - image.height());
                    break;
                case CenterBottom:
                    pos = QPoint(canvas.width()/2 - image.width()/2, canvas.height() - image.height());
                    break;
                case RightBottom:
                    pos = QPoint(canvas.width() - image.width(), canvas.height() - image.height());
                    break;
            }
            painter.drawImage(pos, image);
            painter.end();
            image = canvas;
        }

        foreach (QString filter, m_params->filterList()) {
            if(filter.contains("Oil"))
                image = FilterManager::instance()->oilPaint(image);
            if(filter.contains("Charcoal"))
                image = FilterManager::instance()->charcoal(image);
            if(filter.contains("Swirl"))
                image = FilterManager::instance()->swirl(image);
            if(filter.contains("Solarize"))
                image = FilterManager::instance()->solarize(image);
            if(filter.contains("Wave"))
                image = FilterManager::instance()->wave(image);
            if(filter.contains("Implode"))
                image = FilterManager::instance()->implode(image);
            if(filter.contains("Explode"))
                image = FilterManager::instance()->explode(image);
            if(filter.contains("Soften"))
                image = FilterManager::instance()->soften(image);
            if(filter == "Blur")
                image = FilterManager::instance()->blur(image);
            if(filter.contains("Motion Blur"))
                image = FilterManager::instance()->motionBlur(image);
            if(filter.contains("Sharpen"))
                image = FilterManager::instance()->sharpen(image);
            if(filter.contains("Reinforce"))
                image = FilterManager::instance()->reinforce(image);
            if(filter.contains("Emboss"))
                image = FilterManager::instance()->emboss(image);
            if(filter.contains("Sketch"))
                image = FilterManager::instance()->sketch(image);
            if(filter.contains("Grayscale"))
                image = FilterManager::instance()->grayscale(image);
            if(filter.contains("Black and white"))
                image = FilterManager::instance()->blackwhite(image);
            if(filter.contains("Old Photo"))
                image = FilterManager::instance()->oldPhoto(image);
            if(filter.contains("Sepia"))
                image = FilterManager::instance()->sepia(image);
            if(filter.contains("Negative"))
                image = FilterManager::instance()->negative(image);
            if(filter.contains("Simple Frame"))
                image = FilterManager::instance()->simpleFrame(image);
            if(filter.contains("3D Frame"))
                image = FilterManager::instance()->advFrame(image);
            if(filter.contains("Normalize"))
                image = FilterManager::instance()->normalize(image);
            if(filter.contains("Auto levels"))
                image = FilterManager::instance()->autoLevels(image);
            if(filter.contains("Auto contrast"))
                image = FilterManager::instance()->autoContrast(image);
            if(filter.contains("Crop to center"))
                image = FilterManager::instance()->cropToCenter(image);
            if(filter.contains("Equalize colours"))
                image = FilterManager::instance()->equalizeColours(image);
            if(filter.contains("Monochrome edges"))
                image = FilterManager::instance()->monoChromeEdges(image);
            if(filter.contains("Pixelate"))
                image = FilterManager::instance()->pixelate(image);
            if(filter.contains("Vignette"))
                image = FilterManager::instance()->vignette(image);
            if(filter.contains("Posterize"))
                image = FilterManager::instance()->posterize(image);
            if(filter.contains("Pixel Scatter"))
                image = FilterManager::instance()->pixelScatter(image);
            if(filter.contains("Gaussian noise"))
                image = FilterManager::instance()->gaussianNoise(image);
            if(filter.contains("Impulse noise"))
                image = FilterManager::instance()->impulseNoise(image);
            if(filter.contains("Laplacian noise"))
                image = FilterManager::instance()->laplacianNoise(image);
            if(filter.contains("Poisson noise"))
                image = FilterManager::instance()->poissonNoise(image);
            if(filter.contains("Dust Reduction"))
                image = FilterManager::instance()->dustreduction(image);
            if(filter.contains("Despeckle"))
                image = FilterManager::instance()->deSpeckle(image);
            if(filter.contains("Trim"))
                image = FilterManager::instance()->trim(image);
            if(filter.contains("Drop shadow"))
                image = FilterManager::instance()->dropShadow(image,5,40,Qt::black,0,10);
            if(filter.contains("Opacity"))
                image = FilterManager::instance()->setOpacity(image,0.5);
        }

        switch (m_params->rotate()) {
            case Rotate90CW:
                image = FilterManager::instance()->rotateCW(image);
                break;
            case Rotate90CCW:
                image = FilterManager::instance()->rotateCCW(image);
                break;
            case Rotate180:
                image = FilterManager::instance()->rotateCW(image);
                image = FilterManager::instance()->rotateCW(image);
                break;
            default:
                break;
        }

        switch (m_params->flip()) {
            case FlipVertical:
                image = FilterManager::instance()->flipVert(image);
                break;
            case FlipHorizontal:
                image = FilterManager::instance()->flipHorz(image);
                break;
            default:
                break;
        }

        image = FilterManager::instance()->setBrightness(image, m_params->brightness());
        image = FilterManager::instance()->setSaturation(image, m_params->saturation());
        image = FilterManager::instance()->setContrast(image, m_params->contrast());
        image = FilterManager::instance()->setGamma(image, m_params->gamma());

        QString newFile = m_params->outDir() + "/" + QFileInfo(file).baseName()+m_params->outputFormat();
        emit fileProcessFinished(newFile, image);
        emit batchProgress(++i, m_params->fileList().size());
    }
}

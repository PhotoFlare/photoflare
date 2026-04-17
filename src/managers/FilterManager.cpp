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

/*
  Filter manager class.

  Some of the filters are implemented by ImageMagick through the Magick++ API.
  Others use Qt QGraphics methods.
*/

//#include <QDebug>

#include <QApplication>
#include <Magick++.h>
#include <QBuffer>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QColorSpace>

//#include <QElapsedTimer>

#include "FilterManager.h"

class FilterManagerPrivate
{
public:
    Magick::Image* fromQtImage(const QImage &image) const
    {
        // TODO: May need to convert images pixel by pixel.
        // Also need to take into account whether the image has an alpha channel or not.
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        // Strip the Qt color space profile before saving to PNG. Qt 6 embeds an
        // sRGB ICC chunk which causes GraphicsMagick to read the image as
        // sRGBColorspace. Multi-step filters like charcoal then attempt an
        // internal colorspace conversion that fails with the prebuilt lib.
        // With no profile, GraphicsMagick reads the PNG as plain RGBColorspace.
        QImage stripped = image;
        stripped.setColorSpace(QColorSpace());

        if (!stripped.save(&buffer, "PNG"))
            return nullptr;

        Magick::Blob blob(byteArray.data(), byteArray.length());
        return new Magick::Image(blob);
    }

    QImage toQtImage(Magick::Image *image)
    {
        Magick::Blob blob;
        image->write(&blob);

        QByteArray arr((char *)blob.data(), blob.length());
        QImage img;
        img.loadFromData(arr);

        return img;
    }
};

FilterManager* FilterManager::m_instance = nullptr;

FilterManager::~FilterManager()
{
    delete d;
}

FilterManager *FilterManager::instance()
{
    if (!m_instance)
        m_instance = new FilterManager;

    return m_instance;
}

int FilterManager::changeBrightness( int value, int brightness) {
    return qBound<int>(0, value + brightness * 255 / 100, 255);
}

QImage FilterManager::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
    if(src.isNull()) return QImage();   //No need to do anything else!
    if(!effect) return src;             //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}

QImage FilterManager::colorize(const QImage &image, QColor color, double str, bool useContrast)
{
    QGraphicsColorizeEffect *e = new QGraphicsColorizeEffect;
    e->setColor(color);
    e->setStrength(str);

    if(useContrast)
    {
        QImage modifiedImage = applyEffectToImage(image, e);
        QImage newImage = setContrast(modifiedImage,5);
        return newImage;
    }
    else
    {
        QImage modifiedImage = applyEffectToImage(image, e);
        return modifiedImage;
    }
}

QImage FilterManager::blurImage(const QImage &image, int radius)
{
    QGraphicsBlurEffect *e = new QGraphicsBlurEffect;
    e->setBlurRadius(radius);
    QImage modifiedImage = applyEffectToImage(image, e);

    return modifiedImage;
}

QImage FilterManager::setOpacity(const QImage &image, double dV)
{
    QImage result = image.convertToFormat(QImage::Format_ARGB32);
    const int newAlpha = qRound(dV * 255);
    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int x = 0; x < result.width(); ++x) {
            const QRgb pixel = line[x];
            if (qAlpha(pixel) > 0) {
                line[x] = qRgba(qRed(pixel), qGreen(pixel), qBlue(pixel), newAlpha);
            }
        }
    }
    return result;
}

QImage FilterManager::dropShadow(const QImage &image, int radius, int padding, QColor color, int offsetx, int offsety)
{
    QGraphicsDropShadowEffect *e = new QGraphicsDropShadowEffect;
    e->setColor(color);
    e->setOffset(offsetx,offsety);
    e->setBlurRadius(radius);
    QImage modifiedImage = applyEffectToImage(image, e, padding);

    return modifiedImage;
}

QImage FilterManager::blackwhite(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->type(Magick::BilevelType);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::colourthreshold(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->adaptiveThreshold(image.width(),image.height());

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::oilPaint(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->oilPaint();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::charcoal(const QImage &image)
{
    Magick::Image *magickImage = nullptr;
    try {
        magickImage = d->fromQtImage(image);
        if (!magickImage)
            return image;
        magickImage->charcoal();
        QImage modifiedImage = d->toQtImage(magickImage);
        delete magickImage;
        return modifiedImage;
    } catch (...) {
        delete magickImage;
        return image;
    }
}

QImage FilterManager::swirl(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->swirl(90);

    // Performance testing (filter speed/elapsed time)
    //QElapsedTimer timer;
    //timer.start();
    QImage modifiedImage = d->toQtImage(magickImage);
    //qDebug() << timer.elapsed();
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::solarize(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->solarize();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::wave(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->wave();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::implode(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->implode(0.5);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::soften(const QImage &image)
{
    return blurImage(image,2);
}

QImage FilterManager::blur(const QImage &image)
{
    return blurImage(image,6);
}

QImage FilterManager::sharpen(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->sharpen(0.1,1.0);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::reinforce(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->sharpen(0.5,1.0);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::grayscale(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->modulate(100,0,0);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::oldPhoto(const QImage &image)
{
    QImage modifiedImage = grayscale(image);
    return colorize(modifiedImage,QColor(234, 227, 10),0.3, true);
}

QImage FilterManager::sepia(const QImage &image)
{
    QImage modifiedImage = grayscale(image);
    return colorize(modifiedImage,QColor(255, 170, 0),0.3, true);
}

QImage FilterManager::colorize(const QImage &image, QColor color)
{
    return colorize(image,color,0.5,false);
}

QImage FilterManager::hue(const QImage &image, int degrees)
{
    QImage modifiedImage = image;

    for(int i=0; i<modifiedImage.width(); i++)
    {
        for(int j=0; j<modifiedImage.height(); j++)
        {
            QColor color = modifiedImage.pixelColor(i,j);
            // modify hue as you’d like and write back to the image
            color.setHsv(degrees, color.saturation(), color.value(), color.alpha());
            modifiedImage.setPixelColor(i, j, color);
        }
    }
    return modifiedImage;
}

QImage FilterManager::gradient(const QImage &image, QPoint startPoint, QPoint stopPoint, QColor startColor, QColor stopColor)
{
    QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&surface);
    painter.drawImage(0,0,image);

    QLinearGradient brush(startPoint, stopPoint);
    brush.setColorAt(0.0, startColor);
    brush.setColorAt(1.0, stopColor);

    painter.setBrush(brush);
    painter.fillRect(surface.rect(), brush);

    return surface;
}

QImage FilterManager::fitImage(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->thumbnail(Magick::Geometry(640,400,0,0,false,false));

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::explode(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->spread(25);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::skew(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->shear(1.5,20.5);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::advFrame(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->raise(Magick::Geometry(15,15,0,0,false,false));

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::normalize(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->normalize();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::motionBlur(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->motionBlur(55.5,22.5,1.2);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::simpleFrame(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->frame(15,15,6,6);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::cropToCenter(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->extent(Magick::Geometry(640,400,0,0,false,false),Magick::Color(127,127,127),Magick::StaticGravity);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::equalizeColours(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->equalize();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::monoChromeEdges(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->edge(0.1);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::gaussianNoise(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->addNoise(Magick::GaussianNoise);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::impulseNoise(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->addNoise(Magick::ImpulseNoise);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::laplacianNoise(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->addNoise(Magick::LaplacianNoise);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::poissonNoise(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->addNoise(Magick::PoissonNoise);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::emboss(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->shade(30,30,true);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::trim(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->trim();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::dustreduction(const QImage &image)
{
    const int w = image.width();
    const int h = image.height();

    // Auto-detect a uniform frame border by scanning from each edge inward.
    // A frame added by outsideFrame() is a solid-colour rectangle; we use the
    // top-left corner pixel as the reference colour and a small tolerance to
    // handle slight JPEG compression artifacts.
    QImage img = image.convertToFormat(QImage::Format_ARGB32);
    const QRgb cornerColor = img.pixel(0, 0);
    const int cr = qRed(cornerColor), cg = qGreen(cornerColor), cb = qBlue(cornerColor);
    const int tolerance = 10;

    auto matchesFrame = [&](QRgb px) {
        return qAbs(qRed(px)   - cr) <= tolerance &&
               qAbs(qGreen(px) - cg) <= tolerance &&
               qAbs(qBlue(px)  - cb) <= tolerance;
    };

    int top = 0;
    for (int y = 0; y < h; ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        bool uniform = true;
        for (int x = 0; x < w; ++x)
            if (!matchesFrame(line[x])) { uniform = false; break; }
        if (uniform) ++top; else break;
    }
    int bottom = 0;
    for (int y = h - 1; y >= top; --y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        bool uniform = true;
        for (int x = 0; x < w; ++x)
            if (!matchesFrame(line[x])) { uniform = false; break; }
        if (uniform) ++bottom; else break;
    }
    int left = 0;
    for (int x = 0; x < w; ++x) {
        bool uniform = true;
        for (int y = top; y < h - bottom; ++y) {
            const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
            if (!matchesFrame(line[x])) { uniform = false; break; }
        }
        if (uniform) ++left; else break;
    }
    int right = 0;
    for (int x = w - 1; x >= left; --x) {
        bool uniform = true;
        for (int y = top; y < h - bottom; ++y) {
            const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
            if (!matchesFrame(line[x])) { uniform = false; break; }
        }
        if (uniform) ++right; else break;
    }

    const int contentX = left;
    const int contentY = top;
    const int contentW  = w - left - right;
    const int contentH  = h - top - bottom;

    // Only skip the frame if all four sides have a detected border (the expected
    // shape for outsideFrame()), and the remaining content area is valid.
    if (top > 0 && bottom > 0 && left > 0 && right > 0 && contentW > 0 && contentH > 0) {
        // Filter only the content area; the frame is left completely untouched.
        QImage content = image.copy(contentX, contentY, contentW, contentH);
        Magick::Image *magickImage = d->fromQtImage(content);
        magickImage->reduceNoise();
        QImage filteredContent = d->toQtImage(magickImage);
        delete magickImage;

        QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&result);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawImage(contentX, contentY, filteredContent);
        painter.end();
        return result;
    }

    // No frame detected — apply to the whole image.
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->reduceNoise();
    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;
    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::flipHorz(const QImage &image)
{
    return image.mirrored(true,false);
}

QImage FilterManager::flipVert(const QImage &image)
{
    return image.mirrored(false,true);
}

QImage FilterManager::rotateCCW(const QImage &image)
{
    return image.transformed(QTransform().rotate(270.0));
}

QImage FilterManager::rotateCW(const QImage &image)
{
    return image.transformed(QTransform().rotate(90.0));
}

QImage FilterManager::rotate(const QImage &image, double degrees)
{
    return image.transformed(QTransform().rotate(degrees));
}

QImage FilterManager::setBrightness(const QImage &image, int brightness)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->modulate(brightness + 100.0f, 100.0f, 100.0f);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::setSaturation(const QImage &image, int saturation)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->modulate(100.0f, saturation + 100.0f, 100.0f);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::setContrast(const QImage &image, int contrast)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->contrast(contrast);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::setGamma(const QImage &image, double gamma)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->gamma(gamma, gamma, gamma);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::outsideFrame(const QImage &image, int width, QColor color)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->borderColor(Magick::ColorRGB(color.redF(), color.greenF(), color.blueF()));
    magickImage->border(Magick::Geometry(width,width));

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
}

QImage FilterManager::deSpeckle(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->despeckle();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::negative(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->negate(false);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::autoLevels(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->equalize();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::autoContrast(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->normalize();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    return modifiedImage;
}

QImage FilterManager::floodFill(const QImage &image, const QPoint &pos, const QColor &color)
{
    //RAII - magickImage will be deleted automatically after func return
    QScopedPointer<Magick::Image> magickImage( d->fromQtImage(image) );

    //Workaround to make it work on black color. May be it could be done more gracefully
    bool changeStartColor = (QColor(image.pixel(pos)) == QColor("black")) ? true : false;

    if(changeStartColor)
        magickImage->opaque(Magick::ColorRGB(0, 0, 0), Magick::ColorRGB(0.01f, 0, 0));

    magickImage->floodFillColor(pos.x(), pos.y(), Magick::ColorRGB(color.redF(), color.greenF(), color.blueF()));

    if(changeStartColor)
        magickImage->opaque(Magick::ColorRGB(0.01f, 0, 0), Magick::ColorRGB(0, 0, 0));

    return d->toQtImage(magickImage.data());
}

QPolygon FilterManager::selectArea(const QImage &image, const QPoint &pos, int tolerance, bool color)
{
    //RAII - magickImage will be deleted automatically after func return
    QScopedPointer<Magick::Image> magickImage( d->fromQtImage(image) );
    QScopedPointer<Magick::Image> magickImage2( d->fromQtImage(image) );

    //Workaround to make it work on black color. May be it could be done more gracefully
    bool changeStartColor = (QColor(image.pixel(pos)) == QColor("black")) ? true : false;

    Magick::Color targetColor = magickImage->pixelColor(pos.x(), pos.y());

    if(changeStartColor)
        magickImage->opaque(Magick::ColorRGB(0, 0, 0), Magick::ColorRGB(0.01f, 0, 0));

    magickImage->colorFuzz((double)tolerance / 100.0 * MaxRGBDouble);
    magickImage->floodFillColor(pos.x(), pos.y(), Magick::ColorRGB(0,0,0));

    QPolygon polygon;
    for(unsigned int j=0; j<magickImage->rows(); j++)
    {
        bool marked = false;
        for(unsigned int i=0; i<magickImage->columns(); i++)
        {
            if(color && magickImage->pixelColor(i,j) == targetColor)
            {
                magickImage->floodFillColor(i, j, Magick::ColorRGB(0,0,0));
            }

            if((i == magickImage->columns() - 1) && marked)
                marked = false;

            if( (magickImage->pixelColor(i,j) == Magick::ColorRGB (0, 0, 0) && !marked) || (magickImage->pixelColor(i,j) != Magick::ColorRGB(0, 0, 0) && marked) )
            {
                polygon<<QPoint(i,j);
                break;
            }
        }
    }

    for(int j=magickImage->rows(); j>0; j--)
    {
        bool marked = false;
        for(unsigned int i=magickImage->columns(); i>0; i--)
        {
            if((i == magickImage->columns() - 1) && marked)
                marked = false;

            if((magickImage->pixelColor(i,j) == Magick::ColorRGB(0, 0, 0) && !marked) || (magickImage->pixelColor(i,j) != Magick::ColorRGB(0, 0, 0) && marked) )
            {
                polygon<<QPoint(i,j);
                break;
            }
        }
    }

    if(changeStartColor)
        magickImage->opaque(Magick::ColorRGB(0.01f, 0, 0), Magick::ColorRGB(0, 0, 0));

    return polygon;
}

QImage FilterManager::floodFillOpacity(const QImage &image, const QColor &color, int tolerance)
{
    QScopedPointer<Magick::Image> magickImage( d->fromQtImage(image) );

    Magick::Color targetColor = Magick::ColorRGB(color.redF(), color.greenF(), color.blueF());
    bool changeStartColor = (targetColor == Magick::ColorRGB(0, 0, 0)) ? true : false;
    if(targetColor.alpha() == 1.0f)
        return image;

    magickImage->colorFuzz((double)tolerance / 100.0 * MaxRGBDouble);
    for(unsigned int j=0; j<magickImage->rows(); j++)
    {
        for(unsigned int i=0; i<magickImage->columns(); i++)
        {
            Magick::Color color = magickImage->pixelColor(i,j);
            if(color.alpha() != 1.0f && color == targetColor)
            {
                if(changeStartColor)
                    magickImage->opaque(Magick::ColorRGB(0, 0, 0), Magick::ColorRGB(0.97f, 0.82f, 0.65f));

                magickImage->floodFillColor(i, j, targetColor);

                if(changeStartColor)
                    magickImage->opaque(Magick::ColorRGB(0.97f, 0.82f, 0.65f), Magick::ColorRGB(0, 0, 0));

                magickImage->transparent(targetColor);
            }
        }
    }
    return d->toQtImage(magickImage.data());
}

QImage FilterManager::pixelate(const QImage &image)
{
    if (image.isNull())
        return image;
    const int blockSize = 12;
    QImage small = image.scaled(
        qMax(1, image.width() / blockSize),
        qMax(1, image.height() / blockSize),
        Qt::IgnoreAspectRatio, Qt::FastTransformation);
    return small.scaled(image.width(), image.height(),
                        Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QImage FilterManager::vignette(const QImage &image)
{
    if (image.isNull())
        return image;
    QImage result = image.convertToFormat(QImage::Format_ARGB32);
    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    QRadialGradient gradient(QPointF(result.width() / 2.0, result.height() / 2.0),
                             qMin(result.width(), result.height()) * 0.7);
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 200));
    painter.fillRect(result.rect(), gradient);
    return result;
}

QImage FilterManager::posterize(const QImage &image)
{
    if (image.isNull())
        return image;
    QImage result = image.convertToFormat(QImage::Format_ARGB32);
    const int levels = 4;
    const float step = 255.0f / (levels - 1);
    for (int y = 0; y < result.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(result.scanLine(y));
        for (int x = 0; x < result.width(); ++x) {
            QRgb px = line[x];
            int r = qBound(0, qRound(qRound(qRed(px)   / step) * step), 255);
            int g = qBound(0, qRound(qRound(qGreen(px) / step) * step), 255);
            int b = qBound(0, qRound(qRound(qBlue(px)  / step) * step), 255);
            line[x] = qRgba(r, g, b, qAlpha(px));
        }
    }
    return result;
}

QImage FilterManager::pixelScatter(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->spread(5);
    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;
    return modifiedImage;
}

QImage FilterManager::sketch(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->edge(1.0);
    magickImage->negate();
    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;
    return modifiedImage;
}

FilterManager::FilterManager()
    : d(new FilterManagerPrivate)
{
    Magick::InitializeMagick(qApp->applicationDirPath().toLocal8Bit());
}

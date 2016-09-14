/*

  Filter manager class.

  Some of the filters are implemented by ImageMagick through the Magick++ API.

*/

#include "FilterManager.h"

#include <QApplication>
#include <Magick++.h>
#include <QBuffer>

class FilterManagerPrivate
{
public:
    FilterManagerPrivate()
    {

    }

    ~FilterManagerPrivate()
    {

    }

    Magick::Image* fromQtImage(const QImage &image) const
    {
        // TODO: May need to convert images pixel by pixel.
        // Also need to take into account whether the image has an alpha channel or not.
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);

        if (!image.save(&buffer, "JPG"))
            return 0;

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

FilterManager* FilterManager::m_instance = 0;

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

QImage FilterManager::oilPaint(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->oilPaint();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::charcoal(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->charcoal();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::swirl(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->swirl(90);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::grayscale(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->type(Magick::GrayscaleType);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::flipHorz(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->flop();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::flipVert(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->flip();

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::rotateCCW(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->rotate(270);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

QImage FilterManager::rotateCW(const QImage &image)
{
    Magick::Image *magickImage = d->fromQtImage(image);
    magickImage->rotate(90);

    QImage modifiedImage = d->toQtImage(magickImage);
    delete magickImage;

    // NOTE: Conversion is needed because Magick++ API changes the image's format
    // after applying filter.
    return modifiedImage.convertToFormat(QImage::Format_RGB32);
}

FilterManager::FilterManager()
    : d(new FilterManagerPrivate)
{
    Magick::InitializeMagick(qApp->applicationDirPath().toLocal8Bit());
}

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

/*

  Filter manager class.

  Some of the filters are implemented by ImageMagick through the Magick++ API.

*/

#include <QImage>

class FilterManagerPrivate;

class FilterManager
{
public:
    ~FilterManager();

    static FilterManager* instance();

    QImage oilPaint(const QImage &image);
    QImage charcoal(const QImage &image);
    QImage swirl(const QImage &image);
    QImage grayscale(const QImage &image);
    QImage flipHorz(const QImage &image);
    QImage flipVert(const QImage &image);
    QImage rotateCCW(const QImage &image);
    QImage rotateCW(const QImage &image);

    QImage floodFill(const QImage &image, const QPoint &pos, const QColor &color);
    QPolygon selectArea(const QImage &image, const QPoint &pos, int tolerance, bool color);

private:
    FilterManager();
    FilterManager(const FilterManager &);
    FilterManager& operator=(const FilterManager &);

    static FilterManager* m_instance;

    FilterManagerPrivate *d;
};

#endif // FILTERMANAGER_H

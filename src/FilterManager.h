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

private:
    FilterManager();
    FilterManager(const FilterManager &);
    FilterManager& operator=(const FilterManager &);

    static FilterManager* m_instance;

    FilterManagerPrivate *d;
};

#endif // FILTERMANAGER_H

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

/*

  Filter manager class.

  Some of the filters are implemented by ImageMagick through the Magick++ API.

*/

#include <QImage>

class FilterManagerPrivate;
class ChannelType;

class FilterManager
{
public:
    ~FilterManager();

    static FilterManager* instance();

    QImage oilPaint(const QImage &image);
    QImage charcoal(const QImage &image);
    QImage swirl(const QImage &image);
    QImage solarize(const QImage &image);
    QImage wave(const QImage &image);
    QImage implode(const QImage &image);
    QImage soften(const QImage &image);
    QImage blur(const QImage &image);
    QImage sharpen(const QImage &image);
    QImage reinforce(const QImage &image);
    QImage grayscale(const QImage &image);
    QImage oldPhoto(const QImage &image);
    QImage sepia(const QImage &image);
    QImage dustreduction(const QImage &image);
    QImage flipHorz(const QImage &image);
    QImage flipVert(const QImage &image);
    QImage rotateCCW(const QImage &image);
    QImage rotateCW(const QImage &image);

    QImage setBrightness(const QImage &image, int brightness, int channel);
    QImage setSaturation(const QImage &image, int saturation, int channel);
    QImage setContrast(const QImage &image, int contrast, int channel);
    QImage setGamma(const QImage &image, float gamma, int channel);

    QImage emboss(const QImage &image);
    QImage trim(const QImage &image);
    QImage gaussianNoise(const QImage &image);
    QImage impulseNoise(const QImage &image);
    QImage laplacianNoise(const QImage &image);
    QImage poissonNoise(const QImage &image);
    QImage monoChromeEdges(const QImage &image);
    QImage equalizeColours(const QImage &image);
    QImage cropToCenter(const QImage &image);
    QImage simpleFrame(const QImage &image);
    QImage motionBlur(const QImage &image);
    QImage normalize(const QImage &image);
    QImage advFrame(const QImage &image);
    QImage skew(const QImage &image);
    QImage explode(const QImage &image);
    QImage fitImage(const QImage &image);
    QImage deSpeckle(const QImage &image);
    QImage outsideFrame(const QImage &image, int width);

    QImage autoLevels(const QImage &image);
    QImage autoContrast(const QImage &image);

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

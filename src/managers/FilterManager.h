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

#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <QImage>
#include <QGraphicsEffect>

class FilterManagerPrivate;
class ChannelType;

class FilterManager
{
public:
    ~FilterManager();
    static FilterManager* instance();

    int changeBrightness(int value,int brightness);
    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent = 0);
    QImage blackwhite(const QImage &image);
    QImage colourthreshold(const QImage &image);
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
    QImage colorize(const QImage &image, QColor color);
    QImage hue(const QImage &image, int degrees);
    QImage gradient(const QImage &image, QPoint startPoint, QPoint stopPoint, QColor startColor, QColor stopColor);
    QImage dustreduction(const QImage &image);
    QImage flipHorz(const QImage &image);
    QImage flipVert(const QImage &image);
    QImage rotateCCW(const QImage &image);
    QImage rotateCW(const QImage &image);
    QImage setBrightness(const QImage &image, int brightness);
    QImage setSaturation(const QImage &image, int saturation);
    QImage setContrast(const QImage &image, int contrast);
    QImage setGamma(const QImage &image, double gamma);
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
    QImage outsideFrame(const QImage &image, int width, QColor color);
    QImage autoLevels(const QImage &image);
    QImage autoContrast(const QImage &image);
    QImage floodFill(const QImage &image, const QPoint &pos, const QColor &color);
    QPolygon selectArea(const QImage &image, const QPoint &pos, int tolerance, bool color);
    QImage floodFillOpacity(const QImage &image, const QColor &color, int opacity);
    QImage negative(const QImage &image);
    // Qt GraphicsEffect methods
    QImage dropShadow(const QImage &image, int radius, int padding, QColor color, int offsetx, int offsety);
    QImage setOpacity(const QImage &image, double dV);
    QImage blurImage(const QImage &image, int radius);
    QImage colorize(const QImage &image, QColor color, double str,bool useContrast);

private:
    FilterManager();
    FilterManager(const FilterManager &);
    FilterManager& operator=(const FilterManager &);
    static FilterManager* m_instance;
    FilterManagerPrivate *d;
};

#endif // FILTERMANAGER_H

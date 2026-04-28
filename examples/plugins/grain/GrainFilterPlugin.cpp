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

#include <QObject>
#include <QtPlugin>
#include <QImage>
#include <QRandomGenerator>

#include "../../../src/plugins/IPhotoflarePlugin.h"
#include "../../../src/plugins/AppContext.h"

/**
 * GrainFilterPlugin
 *
 * Adds photographic film grain to the current image.
 * Demonstrates: IFilterPlugin, PluginParam descriptors, apply(), preview().
 */
class GrainFilterPlugin : public QObject, public IFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.photoflare.FilterPlugin/1.0" FILE "grain.json")
    Q_INTERFACES(IPhotoflarePlugin IFilterPlugin)

public:
    // ── Identity ─────────────────────────────────────────────────────────────

    QString    pluginId()    const override { return "org.photoflare.filters.grain"; }
    QString    displayName() const override { return "Film Grain"; }
    QString    version()     const override { return "1.0.0"; }
    QString    author()      const override { return "Photoflare Contributors"; }
    QString    menuPath()    const override { return "Filters/Noise"; }

    void initialise(AppContext* ctx) override { m_ctx = ctx; }

    // ── Parameters ───────────────────────────────────────────────────────────

    QList<PluginParam> parameters() const override
    {
        return {
            { "intensity",  "Intensity",   PluginParam::Float, 0.3,  0.0, 1.0, {} },
            { "size",       "Grain Size",  PluginParam::Float, 1.0,  0.5, 4.0, {} },
            { "monochrome", "Monochrome",  PluginParam::Bool,  false, {}, {},  {} },
        };
    }

    // ── Preview (fast — reduced resolution) ──────────────────────────────────

    QImage preview(const QImage& input, const QVariantMap& params) const override
    {
        QImage small = (input.width() > 800 || input.height() > 800)
            ? input.scaled(800, 800, Qt::KeepAspectRatio, Qt::FastTransformation)
            : input;
        return apply(small, params);
    }

    // ── Apply ─────────────────────────────────────────────────────────────────

    QImage apply(const QImage& input, const QVariantMap& params) const override
    {
        const float intensity  = params.value("intensity",  0.3f).toFloat();
        const float grainSize  = params.value("size",       1.0f).toFloat();
        const bool  monochrome = params.value("monochrome", false).toBool();

        QImage result = input.convertToFormat(QImage::Format_ARGB32);
        const int w = result.width();
        const int h = result.height();

        for (int y = 0; y < h; ++y) {
            QRgb* line = reinterpret_cast<QRgb*>(result.scanLine(y));
            for (int x = 0; x < w; ++x) {
                // Snap to grain cell for coarser grain sizes
                const int gx = static_cast<int>(x / grainSize);
                const int gy = static_cast<int>(y / grainSize);
                QRandomGenerator rng(42 + gy * 10000 + gx);

                const int alpha = qAlpha(line[x]);
                int r = qRed(line[x]);
                int g = qGreen(line[x]);
                int b = qBlue(line[x]);

                if (monochrome) {
                    const int noise = static_cast<int>((rng.generateDouble() - 0.5) * 255.0 * intensity);
                    r = qBound(0, r + noise, 255);
                    g = qBound(0, g + noise, 255);
                    b = qBound(0, b + noise, 255);
                } else {
                    r = qBound(0, r + static_cast<int>((rng.generateDouble() - 0.5) * 255.0 * intensity), 255);
                    g = qBound(0, g + static_cast<int>((rng.generateDouble() - 0.5) * 255.0 * intensity), 255);
                    b = qBound(0, b + static_cast<int>((rng.generateDouble() - 0.5) * 255.0 * intensity), 255);
                }
                line[x] = qRgba(r, g, b, alpha);
            }
        }
        return result;
    }

private:
    AppContext* m_ctx = nullptr;
};

// Required when Q_OBJECT is used in a .cpp file
#include "GrainFilterPlugin.moc"

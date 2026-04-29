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
#include <QImageWriter>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QWidget>

#include "../../../src/plugins/IPhotoflarePlugin.h"
#include "../../../src/plugins/AppContext.h"

/**
 * WebpExporterPlugin
 *
 * Exports images as WebP using Qt 6's built-in WebP codec — no external
 * tools or libraries required.
 *
 * Settings:
 *   quality    — 0–100 (maps to QImageWriter::setQuality)
 *   lossless   — bool (maps to QImageWriter::setSubType("lossless"))
 */
class WebpExporterPlugin : public QObject, public IExporterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.photoflare.ExporterPlugin/1.0" FILE "webp.json")
    Q_INTERFACES(IPhotoflarePlugin IExporterPlugin)

public:
    // ── Identity ──────────────────────────────────────────────────────────────

    QString    pluginId()    const override { return "org.photoflare.exporters.webp"; }
    QString    displayName() const override { return "WebP Exporter"; }
    QString    version()     const override { return "1.0.0"; }
    QString    author()      const override { return "Photoflare Contributors"; }
    QString    formatName()  const override { return "WebP Image"; }

    QStringList supportedExtensions() const override { return { "webp" }; }

    void initialise(AppContext* ctx) override { m_ctx = ctx; }

    // ── Settings widget ───────────────────────────────────────────────────────

    QWidget* settingsWidget(QWidget* parent) const override
    {
        auto* w      = new QWidget(parent);
        auto* layout = new QFormLayout(w);

        auto* qualitySpin = new QSpinBox(w);
        qualitySpin->setRange(1, 100);
        qualitySpin->setValue(80);
        qualitySpin->setSuffix("%");
        qualitySpin->setObjectName("quality");
        layout->addRow("Quality:", qualitySpin);

        auto* losslessCheck = new QCheckBox(w);
        losslessCheck->setObjectName("lossless");
        layout->addRow("Lossless:", losslessCheck);

        w->setLayout(layout);
        return w;
    }

    // ── Export ────────────────────────────────────────────────────────────────

    bool exportImage(const QImage&      image,
                     const QString&     filePath,
                     const QVariantMap& settings,
                     QString&           errorMessage) const override
    {
        const int  quality  = settings.value("quality",  80).toInt();
        const bool lossless = settings.value("lossless", false).toBool();

        QImageWriter writer(filePath, "webp");
        writer.setQuality(quality);
        if (lossless)
            writer.setSubType("lossless");

        if (!writer.write(image)) {
            errorMessage = writer.errorString();
            return false;
        }
        return true;
    }

private:
    AppContext* m_ctx = nullptr;
};

// Required when Q_OBJECT is defined in a .cpp file
#include "WebpExporterPlugin.moc"

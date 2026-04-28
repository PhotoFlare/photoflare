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

#ifndef IPHOTOFLARE_PLUGIN_H
#define IPHOTOFLARE_PLUGIN_H

#include <QtPlugin>
#include <QString>
#include <QImage>
#include <QWidget>
#include <QVariantMap>
#include <QList>
#include <QStringList>

class AppContext;

// ─────────────────────────────────────────────
// Plugin parameter descriptor (drives auto-generated UI)
// ─────────────────────────────────────────────
struct PluginParam {
    enum Type { Float, Int, Bool, Colour, Choice };

    QString     id;           // internal key,  e.g. "intensity"
    QString     label;        // display label, e.g. "Intensity"
    Type        type;
    QVariant    defaultValue;
    QVariant    minValue;     // Float / Int only
    QVariant    maxValue;     // Float / Int only
    QStringList choices;      // Choice type only
};

// ─────────────────────────────────────────────
// Plugin type tag
// ─────────────────────────────────────────────
enum class PluginType {
    Filter,    // QImage → QImage transformation
    Exporter   // writes QImage to a file format
    // Tool plugins are out of scope for v1
};

// ─────────────────────────────────────────────
// Base interface — every plugin implements this
// ─────────────────────────────────────────────
class IPhotoflarePlugin {
public:
    virtual ~IPhotoflarePlugin() = default;

    virtual QString    pluginId()    const = 0;  // reverse-DNS, e.g. "org.photoflare.grain"
    virtual QString    displayName() const = 0;
    virtual QString    version()     const = 0;
    virtual QString    author()      const = 0;
    virtual PluginType type()        const = 0;

    // Called once after load; ctx is valid for the plugin's lifetime.
    virtual void initialise(AppContext* ctx) = 0;

    // Called before unload.
    virtual void shutdown() {}
};

Q_DECLARE_INTERFACE(IPhotoflarePlugin, "org.photoflare.Plugin/1.0")


// ─────────────────────────────────────────────
// Filter plugin interface
// ─────────────────────────────────────────────
class IFilterPlugin : public IPhotoflarePlugin {
public:
    PluginType type() const final { return PluginType::Filter; }

    // Parameters exposed to the host's auto-generated dialog.
    // Keys in the QVariantMap passed to apply() match PluginParam::id.
    virtual QList<PluginParam> parameters() const = 0;

    // Menu location, e.g. "Filters/Noise" → Filters > Noise submenu.
    virtual QString menuPath() const = 0;

    // Apply the filter. Must NOT modify the input; return a new QImage.
    virtual QImage apply(const QImage& input, const QVariantMap& params) const = 0;

    // Optional fast preview (e.g. work at reduced resolution).
    virtual QImage preview(const QImage& input, const QVariantMap& params) const {
        return apply(input, params);
    }
};

Q_DECLARE_INTERFACE(IFilterPlugin, "org.photoflare.FilterPlugin/1.0")


// ─────────────────────────────────────────────
// Exporter plugin interface
// ─────────────────────────────────────────────
class IExporterPlugin : public IPhotoflarePlugin {
public:
    PluginType type() const final { return PluginType::Exporter; }

    // File extensions this plugin handles, e.g. {"avif", "avifs"}.
    virtual QStringList supportedExtensions() const = 0;

    // Human-readable name for the Save dialog filter string, e.g. "AVIF Image".
    virtual QString formatName() const = 0;

    // Optional settings widget shown in the export dialog for this format.
    // Caller takes ownership. Return nullptr if no settings are needed.
    virtual QWidget* settingsWidget(QWidget* parent) const { Q_UNUSED(parent); return nullptr; }

    // Write image to filePath. settings come from settingsWidget() values.
    // Returns false and sets errorMessage on failure.
    virtual bool exportImage(const QImage&    image,
                             const QString&   filePath,
                             const QVariantMap& settings,
                             QString&         errorMessage) const = 0;
};

Q_DECLARE_INTERFACE(IExporterPlugin, "org.photoflare.ExporterPlugin/1.0")

#endif // IPHOTOFLARE_PLUGIN_H

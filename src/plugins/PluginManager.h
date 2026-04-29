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

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>
#include <QJsonObject>

#include "IPhotoflarePlugin.h"
#include "AppContext.h"

/**
 * PluginManager
 *
 * Owns all QPluginLoaders and provides typed access to loaded plugins.
 * Instantiate once in MainWindow; pass your AppContext pointer.
 *
 * Usage:
 *   m_pluginManager = new PluginManager(this, this);
 *   m_pluginManager->scanDirectories({
 *       qApp->applicationDirPath() + "/plugins",
 *       QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/plugins"
 *   });
 */
class PluginManager : public QObject {
    Q_OBJECT

public:
    explicit PluginManager(AppContext* ctx, QObject* parent = nullptr)
        : QObject(parent), m_ctx(ctx) {}

    ~PluginManager() { unloadAll(); }

    // ── Discovery ────────────────────────────────────────────────────────────

    void scanDirectories(const QStringList& paths) {
        m_scannedPaths = paths;
        m_errors.clear();

#if defined(Q_OS_WIN)
        const QStringList nameFilters = { "*.dll" };
#elif defined(Q_OS_MAC)
        const QStringList nameFilters = { "*.dylib" };
#else
        const QStringList nameFilters = { "*.so" };
#endif

        for (const QString& path : paths) {
            QDir dir(path);
            if (!dir.exists()) {
                m_errors << QString("[not found] %1").arg(dir.absolutePath());
                continue;
            }
            const QStringList entries = dir.entryList(nameFilters, QDir::Files);
            if (entries.isEmpty())
                m_errors << QString("[no plugins] %1").arg(dir.absolutePath());
            for (const QString& filename : entries)
                tryLoad(dir.absoluteFilePath(filename));
        }
        emit pluginsLoaded();
    }

    // ── Diagnostics ──────────────────────────────────────────────────────────

    QStringList scannedPaths() const { return m_scannedPaths; }
    QStringList loadErrors()   const { return m_errors; }

    // ── Typed accessors ──────────────────────────────────────────────────────

    QList<IFilterPlugin*>   filterPlugins()   const { return m_filters;   }
    QList<IExporterPlugin*> exporterPlugins() const { return m_exporters; }

    QList<IPhotoflarePlugin*> allPlugins() const {
        QList<IPhotoflarePlugin*> all;
        for (auto* p : m_filters)   all << p;
        for (auto* p : m_exporters) all << p;
        return all;
    }

    // ── Metadata peek (no full load) ─────────────────────────────────────────

    static QJsonObject peekMetadata(const QString& filePath) {
        QPluginLoader loader(filePath);
        return loader.metaData().value("MetaData").toObject();
    }

    // ── Unload ───────────────────────────────────────────────────────────────

    void unloadAll() {
        for (auto* p : allPlugins())
            p->shutdown();
        for (QPluginLoader* loader : m_loaders) {
            loader->unload();
            delete loader;
        }
        m_loaders.clear();
        m_filters.clear();
        m_exporters.clear();
    }

signals:
    void pluginsLoaded();
    void pluginLoadError(const QString& filePath, const QString& error);

private:
    void tryLoad(const QString& filePath) {
        auto* loader = new QPluginLoader(filePath, this);
        QObject* obj = loader->instance();
        if (!obj) {
            const QString err = QString("[error] %1\n  %2").arg(filePath, loader->errorString());
            m_errors << err;
            emit pluginLoadError(filePath, loader->errorString());
            delete loader;
            return;
        }

        auto* plugin = qobject_cast<IPhotoflarePlugin*>(obj);
        if (!plugin) {
            const QString err = QString("[bad interface] %1").arg(filePath);
            m_errors << err;
            emit pluginLoadError(filePath, "Not an IPhotoflarePlugin");
            loader->unload();
            delete loader;
            return;
        }

        plugin->initialise(m_ctx);
        m_loaders << loader;

        if (auto* f = qobject_cast<IFilterPlugin*>(obj))
            m_filters << f;
        else if (auto* e = qobject_cast<IExporterPlugin*>(obj))
            m_exporters << e;
    }

    AppContext*              m_ctx;
    QStringList             m_scannedPaths;
    QStringList             m_errors;
    QList<QPluginLoader*>   m_loaders;
    QList<IFilterPlugin*>   m_filters;
    QList<IExporterPlugin*> m_exporters;
};

#endif // PLUGIN_MANAGER_H

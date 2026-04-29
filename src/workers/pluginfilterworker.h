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

#ifndef PLUGINFILTERWORKER_H
#define PLUGINFILTERWORKER_H

#include <QObject>
#include <QImage>
#include <QVariantMap>
#include "../plugins/IPhotoflarePlugin.h"

class PluginFilterWorker : public QObject
{
    Q_OBJECT
public:
    explicit PluginFilterWorker(IFilterPlugin *plugin,
                                const QImage &image,
                                const QVariantMap &params,
                                QObject *parent = nullptr);

public slots:
    void process();

signals:
    void filterProcessFinished(QImage image);

private:
    IFilterPlugin  *m_plugin;
    QImage          m_image;
    QVariantMap     m_params;
};

#endif // PLUGINFILTERWORKER_H

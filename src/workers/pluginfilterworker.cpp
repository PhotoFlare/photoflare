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

#include "pluginfilterworker.h"

PluginFilterWorker::PluginFilterWorker(IFilterPlugin *plugin,
                                       const QImage &image,
                                       const QVariantMap &params,
                                       QObject *parent)
    : QObject(parent)
    , m_plugin(plugin)
    , m_image(image)
    , m_params(params)
{
}

void PluginFilterWorker::process()
{
    QImage result;
    try {
        result = m_plugin->apply(m_image, m_params);
    } catch (...) {
        result = m_image; // fall back to original on error
    }
    emit filterProcessFinished(result);
}

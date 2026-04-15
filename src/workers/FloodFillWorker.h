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

#ifndef FLOODFILLWORKER_H
#define FLOODFILLWORKER_H

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QColor>

class FloodFillWorker : public QObject
{
    Q_OBJECT
public:
    explicit FloodFillWorker(QObject *parent = nullptr);

    void setImage(const QImage &image);
    void setPos(const QPoint &pos);
    void setColor(const QColor &color);

public slots:
    void process();

signals:
    void finished(QImage image);

private:
    QImage m_image;
    QPoint m_pos;
    QColor m_color;
};

#endif // FLOODFILLWORKER_H

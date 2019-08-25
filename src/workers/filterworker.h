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

#ifndef FILTERWORKER_H
#define FILTERWORKER_H

#include <QObject>
#include "../mainwindow.h"

class FilterWorker : public QObject
{
    Q_OBJECT
public:
    //FilterWorker();
    explicit FilterWorker(QObject *parent = 0);
    void setParent(MainWindow* parent){m_parent = parent;}
    void setImage(QImage image);
    void setFilter(QString filter);
    void setDoubleVal(double v);

public slots:
    void process();

signals:
    void filterProcessFinished(QImage image);

private:
    MainWindow* m_parent;
    QImage currentImage;
    QString currentFilter;
    double currentDouble;
};

#endif // FILTERWORKER_H

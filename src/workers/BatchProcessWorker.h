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

#ifndef BATCHPROCESSWORKER_H
#define BATCHPROCESSWORKER_H

#include <QObject>

#include "batchdialog.h"
#include "../mainwindow.h"

class BatchProcessWorker : public QObject
{
    Q_OBJECT
public:
    explicit BatchProcessWorker(QObject *parent = 0);

    void setParams(batchDialog* params){m_params = params;}
    void setParent(MainWindow* parent){m_parent = parent;}
public slots:
    void process();

signals:

    void fileProcessFinished(QString file, QImage image);
    void batchProgress(int idx, int total);
private:
    batchDialog* m_params;
    MainWindow* m_parent;
};

#endif // BATCHPROCESSWORKER_H

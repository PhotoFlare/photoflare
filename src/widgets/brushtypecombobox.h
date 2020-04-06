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

#ifndef BRUSHTYPECOMBOBOX_H
#define BRUSHTYPECOMBOBOX_H

#include <QComboBox>

class QComboBoxEventsInterface
{
public:
    virtual void mousePressEvent(QComboBox* obj,QMouseEvent *e) = 0;
};

class BrushTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit BrushTypeComboBox(QWidget *parent = nullptr);
    ~BrushTypeComboBox();

    void setOnClickHandler(QComboBoxEventsInterface *handler);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent *e);
private:
    QComboBoxEventsInterface *onClickHandler;
};

#endif // BRUSHTYPECOMBOBOX_H

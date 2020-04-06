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

#ifndef IMAGEPOSITIONWIDGET_H
#define IMAGEPOSITIONWIDGET_H

#include <QWidget>

namespace Ui {
class ImagePositionWidget;
}

enum ImagePosition {
    LeftTop,
    CenterTop,
    RightTop,
    LeftCenter,
    CenterCenter,
    RightCenter,
    LeftBottom,
    CenterBottom,
    RightBottom
};

class ImagePositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePositionWidget(QWidget *parent = nullptr);
    ~ImagePositionWidget();

    ImagePosition imagePosition(){ return position; }

private slots:

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();

private:
    Ui::ImagePositionWidget *ui;
    ImagePosition position;
    QString iconSepia;
    QString iconGray;

    void resetAllButtons();
};

#endif // IMAGEPOSITIONWIDGET_H

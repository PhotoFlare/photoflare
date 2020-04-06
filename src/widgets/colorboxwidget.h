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

#ifndef COLORBOXWIDGET_H
#define COLORBOXWIDGET_H

#include <QWidget>

namespace Ui {
class ColorBoxWidget;
}

class ColorBoxWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorBoxWidget(QWidget *parent = nullptr);
    ~ColorBoxWidget() override;

    QColor primaryColor() const;
    QColor secondaryColor() const;

    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void primaryColorChanged(const QColor &color);
    void secondaryColorChanged(const QColor &color);

private slots:
    void on_swapColorButton_clicked();
    void on_prevPage_clicked();
    void on_nextPage_clicked();
    void on_colourManageButton_clicked();

private:
    void showColorDialog(QWidget *colorLabel);
    void setColor(const QColor &color, QWidget *colorLabel);
    QColor getColorFromLabel(QWidget *colorLabel) const;

    Ui::ColorBoxWidget *ui;
};

#endif // COLORBOXWIDGET_H

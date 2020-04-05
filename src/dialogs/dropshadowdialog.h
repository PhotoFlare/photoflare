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

#ifndef DROPSHADOWDIALOG_H
#define DROPSHADOWDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QMouseEvent>

#include <brushtypecombobox.h>

namespace Ui {
class dropshadowDialog;
}

class dropshadowDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit dropshadowDialog(QWidget *parent = nullptr);
    ~dropshadowDialog();

    void mousePressEvent(QComboBox* obj, QMouseEvent *e);
    QColor color() const;
    int radius() const;
    int padding() const;
    int offsetx() const;
    int offsety() const;

    void on_buttonBox_accepted();

private slots:
    void on_defaultsButton_clicked();

private:
    Ui::dropshadowDialog *ui;

    QColor colorName();
    void setComboColor(QComboBox *obj, QColor c);
    void writeSettings(QWidget* window);
    void readSettings(QWidget* window);
};

#endif // DROPSHADOWDIALOG_H

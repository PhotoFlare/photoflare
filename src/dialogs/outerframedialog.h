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

#ifndef OUTERFRAMEDIALOG_H
#define OUTERFRAMEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QMouseEvent>

#include <brushtypecombobox.h>

namespace Ui {
class OuterFrameDialog;
}

class OuterFrameDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit OuterFrameDialog(QWidget *parent = 0);
    ~OuterFrameDialog();

    void mousePressEvent(QComboBox* obj, QMouseEvent *e);
    QColor color() const;
    int width() const;
private:
    Ui::OuterFrameDialog *ui;
};

#endif // OUTERFRAMEDIALOG_H

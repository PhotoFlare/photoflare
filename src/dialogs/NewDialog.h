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

#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <brushtypecombobox.h>
#include "imagepositionwidget.h"

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = nullptr);
    ~NewDialog();

    /*!
     * This method returns the size of new image that was chosen by
     * user. Usually it should be called after the dialog was
     * accepted, otherwise you will get an invalid size.
     */
    QSize newImageSize() const;
    void setImageSize(QSize size);
    QColor newImageBackgroundColor() const;
    QColor backgroundColor() const;
    bool backgroundColorEnabled();
    enum Mode {ResizeImage, ResizeCanvas, NewImage};
    void setMode(Mode mode);
    ImagePosition imagePosition();
    void onComboBoxPressed();
    //QComboBoxEventsInterface methods
    virtual void mousePressEvent(QComboBox* obj,QMouseEvent *e);
private slots:
    void on_buttonBox_accepted();
    void on_imagePresetCombo_currentIndexChanged(int index);
    void on_imageResCombo_currentIndexChanged(int index);
    void on_imageWHcombo_currentIndexChanged(int index);
    void on_imageRvalue_valueChanged(double arg1);
    void on_imageHvalue_valueChanged(double arg1);
    void on_imageWvalue_valueChanged(double arg1);
    void writeSettings(QWidget* window);
    void readSettings(QWidget* window);

private:
    Ui::NewDialog *ui;

    QSize m_chosenSize;

    double imageRatio;
    float width_px;
    float height_px;
    float original_width_px;
    float original_height_px;
    int currentUnit;
};

#endif // NEWDIALOG_H

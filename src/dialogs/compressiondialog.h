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

#ifndef COMPRESSIONDIALOG_H
#define COMPRESSIONDIALOG_H

#include <QDialog>

namespace Ui {
class CompressionDialog;
}

class CompressionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompressionDialog(QWidget *parent = nullptr);
    ~CompressionDialog();

    int quality() const;
    bool enableSaveImage;
private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::CompressionDialog *ui;
};

#endif // COMPRESSIONDIALOG_H

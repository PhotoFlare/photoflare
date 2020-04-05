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

#ifndef IMAGEPROPERTIESDIALOG_H
#define IMAGEPROPERTIESDIALOG_H

#include <QDialog>

class ImagePropertiesPrivate;

namespace Ui {
class imagePropertiesDialog;
}

class imagePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit imagePropertiesDialog(QWidget *parent = nullptr);
    ~imagePropertiesDialog();

    void setImageName(QString);
    void setFolder(QString);
    void setSize(QSize);
    void setColorCount(QImage);

    void setFileSize(qint64);
    void setDate(QString);

    void setMemorySize(qint64);
    void setTotalSize(qint64);
private:
    Ui::imagePropertiesDialog *ui;
};

#endif // IMAGEPROPERTIESDIALOG_H

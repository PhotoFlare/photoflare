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

#ifndef TRANSPARENTDIALOG_H
#define TRANSPARENTDIALOG_H

#include <QDialog>

namespace Ui {
class TransparentDialog;
}

class TransparentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransparentDialog(QWidget *parent = 0);
    ~TransparentDialog();

    void setColor(QRgb color);
    QColor color() {return m_color;}
    int tolerance();
private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void readSettings(QWidget* window);
    void writeSettings(QWidget* window);

signals:
    void previewTransparent(QColor,int);
    void dialogAccepted();
    void dialogRejected();
    void dialogFinished(int);

protected:
    void closeEvent (QCloseEvent *event);

private:
    Ui::TransparentDialog *ui;
    QColor m_color;
};

#endif // TRANSPARENTDIALOG_H

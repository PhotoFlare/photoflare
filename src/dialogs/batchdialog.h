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

#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>
#include <brushtypecombobox.h>
#include "imagepositionwidget.h"

class BatchDialogPrivate;

enum {Rotate90CW=1, Rotate90CCW, Rotate180};
enum {FlipVertical=1, FlipHorizontal};

namespace Ui {
class batchDialog;
}

class batchDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit batchDialog(QWidget *parent = 0);
    ~batchDialog();

    QStringList fileList();
    QStringList filterList();
    QString outDir();
    QString outputFormat();
    void onFinished();

    bool changeImageSize() const;
    bool changeCanvasSize() const;

    QSize imageSize() const;
    QSize canvasSize() const;
    QColor backgroundColor() const;
    ImagePosition imagePosition() const;
    char imageSizeUnits() const;

    char rotate() const;
    char flip() const;

    int brightness() const;
    int brightnessChannel() const;

    int contrast() const;
    int contrastChannel() const;

    int saturation() const;
    int saturationChannel() const;

    double gamma() const;
    int gammaChannel() const;

private slots:

    void on_listWidget_2_doubleClicked(const QModelIndex &index);

    void on_listWidget_3_doubleClicked(const QModelIndex &index);

    void on_tabWidget_currentChanged(int index);

    void on_imageSizeCheckBox_clicked(bool checked);

    void on_canvasSizeCheckBox_clicked(bool checked);

    void on_imageWHcombo_currentIndexChanged(int index);

    void on_imageWvalue_valueChanged(double arg1);

    void on_imageHvalue_valueChanged(double arg1);

    //QComboBoxEventsInterface methods
    virtual void mousePressEvent(QComboBox* obj, QMouseEvent *e);

    void on_rotateCheckBox_clicked(bool checked);

    void on_flipCheckBox_clicked(bool checked);

    void on_addFilesButton_clicked();

    void on_outFolderPushButton_clicked();

    void on_brightnessSlider_valueChanged(int value);

    void on_contrastSlider_valueChanged(int value);

    void on_saturationSlider_valueChanged(int value);

    void on_gammaSlider_valueChanged(int value);

    void done(int r);

    void writeSettings(QWidget* window);
    void readSettings(QWidget* window);

    void on_outFormat_currentIndexChanged(int index);

    void on_sourceFormat_currentIndexChanged(int index);

    void addItemToFileListWidget(QString filePath);

    void on_addFilterButton_clicked();

    void on_removeFilterButton_clicked();

private:
    Ui::batchDialog *ui;
    BatchDialogPrivate *d;

    QStringList original_list;
};

#endif // BATCHDIALOG_H

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

#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>

namespace Ui {
class prefsDialog;
}

class prefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit prefsDialog(QWidget *parent = nullptr);
    ~prefsDialog();

private slots:
    void on_buttonBox_accepted();
    void on_openFolderButton_clicked();
    void on_saveFolderButton_clicked();
    void on_compressionSlider_sliderMoved(int position);
    void on_historySlider_sliderMoved(int position);
    void on_restartButton_clicked();
    void set_user_language();
    void on_comboBoxLanguage_currentIndexChanged();
    void on_buttonBox_rejected();
    void on_compressionSlider_valueChanged(int value);
    void on_historySlider_valueChanged(int value);
    void addFlagIcons(int languages);

private:
    Ui::prefsDialog *ui;
    QString flagPath;
};

#endif // PREFSDIALOG_H

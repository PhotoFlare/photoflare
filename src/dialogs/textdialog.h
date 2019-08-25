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

#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
class textDialog;
}

class textDialog : public QDialog
{
    Q_OBJECT

public:
    explicit textDialog(QWidget *parent = 0);
    ~textDialog();

    QString text() {return currentText;}
    QFont font() {return currentFont;}
    QColor color() {return currentColor;}
    bool antialias() {return antialiasEnabled;}
    QString position() {return positionFont;}

    void editText(const QString &text, const QFont &font, const QColor &color);
private slots:
    void updateFont();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_fontSizeSpinner_valueChanged(int value);
    void on_checkBoxBold_toggled(bool checked);
    void on_checkBoxItalic_toggled(bool checked);
    void on_checkBoxStrike_toggled(bool checked);
    void on_checkBoxUnderline_toggled(bool checked);
    void on_checkBoxAntialias_toggled(bool checked);
    void on_buttonBoxtextDialog_accepted();
    void on_buttonBoxtextDialog_rejected();
    void setColor(const QColor &color, QWidget *colorLabel);
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showColorDialog(QWidget *colorLabel);
    QColor getColorFromLabel(QWidget *colorLabel) const;
    void readSettings(QWidget* window);
    void writeSettings(QWidget* window);

    void on_alignleftButton_clicked();
    void on_aligncenterButton_clicked();
    void on_alignRightButton_clicked();
    void setFontPosition(QString pos);

signals:
    void textAdded(const QString&, const QFont&);

private:
    Ui::textDialog *ui;

    QString currentText;
    QFont currentFont;
    QColor currentColor;
    bool antialiasEnabled;
    QString positionFont = "left";
};

#endif // TEXTDIALOG_H

#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

/*

  Text dialog class for the PhotoFiltre LX application.

*/

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

    void editText(const QString &text, const QFont &font);
private slots:
    void updateFont();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_fontSizeSpinner_valueChanged(int value);
    void on_checkBoxBold_toggled(bool checked);
    void on_checkBoxItalic_toggled(bool checked);
    void on_checkBoxStrike_toggled(bool checked);
    void on_checkBoxUnderline_toggled(bool checked);
    void on_buttonBoxtextDialog_accepted();
    void on_buttonBoxtextDialog_rejected();

signals:
    void textAdded(const QString&, const QFont&);

private:
    Ui::textDialog *ui;

    QString currentText;
    QFont currentFont;
};

#endif // TEXTDIALOG_H

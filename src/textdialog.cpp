/*

  Text dialog class for the PhotoFiltre LX application.

*/


#include "textdialog.h"
#include "ui_textdialog.h"

textDialog::textDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textDialog)
{
    ui->setupUi(this);
    updateFont();

    const QStringList colourNames = QColor::colorNames();
    int index = 0;
    foreach (const QString &colourName, colourNames) {
        const QColor color(colourName);
        ui->comboBox->addItem(colourName, color);
        const QModelIndex idx = ui->comboBox->model()->index(index++, 0);
        ui->comboBox->model()->setData(idx, color, Qt::BackgroundColorRole);
    }

}

textDialog::~textDialog()
{
    delete ui;
}

void textDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    currentFont = f;
    currentFont.setPixelSize(ui->fontSizeSpinner->value());
    ui->textPreview->setFont(currentFont);
}

void textDialog::updateFont()
{
    currentFont = ui->fontComboBox->font();
    currentFont.setPixelSize(ui->fontSizeSpinner->value());
    currentFont.setBold(ui->checkBoxBold->isChecked());
    currentFont.setItalic(ui->checkBoxItalic->isChecked());
    currentFont.setStrikeOut(ui->checkBoxStrike->isChecked());
    currentFont.setUnderline(ui->checkBoxUnderline->isChecked());
    ui->textPreview->setFont(currentFont);
}

void textDialog::editText(const QString &text, const QFont &font)
{
    ui->fontComboBox->setCurrentFont(font);
    ui->fontSizeSpinner->setValue(font.pixelSize());
    ui->checkBoxBold->setChecked(font.bold());
    ui->checkBoxItalic->setChecked(font.italic());
    ui->checkBoxStrike->setChecked(font.strikeOut());
    ui->checkBoxUnderline->setChecked(font.underline());
    ui->plainTextEdit->setPlainText(text);
    ui->textPreview->setFont(font);
    currentFont = font;
}

void textDialog::on_fontSizeSpinner_valueChanged(int value)
{
    Q_UNUSED(value);
    updateFont();
}

void textDialog::on_checkBoxBold_toggled(bool checked)
{
    Q_UNUSED(checked);
    updateFont();
}

void textDialog::on_checkBoxItalic_toggled(bool checked)
{
    Q_UNUSED(checked);
    updateFont();
}

void textDialog::on_checkBoxStrike_toggled(bool checked)
{
    Q_UNUSED(checked);
    updateFont();
}

void textDialog::on_checkBoxUnderline_toggled(bool checked)
{
    Q_UNUSED(checked);
    updateFont();
}

void textDialog::on_buttonBoxtextDialog_accepted()
{
    currentText = ui->plainTextEdit->toPlainText();
    accept();
}

void textDialog::on_buttonBoxtextDialog_rejected()
{
    close();
}

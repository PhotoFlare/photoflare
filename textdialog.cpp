#include "textdialog.h"
#include "ui_textdialog.h"

textDialog::textDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textDialog)
{
    ui->setupUi(this);
}

textDialog::~textDialog()
{
    delete ui;
}

void textDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont currentFont = f;
    currentFont.setPixelSize(ui->fontSizeSpinner->value());
    ui->textPreview->setFont(currentFont);
}

void textDialog::updateFont()
{
    QFont currentFont = ui->fontComboBox->font();
    currentFont.setPixelSize(fontSize);
    currentFont.setBold(boldState);
    currentFont.setItalic(italicState);
    currentFont.setStrikeOut(strikeState);
    currentFont.setUnderline(underlineState);
    ui->textPreview->setFont(currentFont);
}

void textDialog::on_fontSizeSpinner_valueChanged(int value)
{
    fontSize = value;
    updateFont();
}

void textDialog::on_checkBoxBold_toggled(bool checked)
{
    boldState = checked;
    updateFont();
}

void textDialog::on_checkBoxItalic_toggled(bool checked)
{
    italicState = checked;
    updateFont();
}

void textDialog::on_checkBoxStrike_toggled(bool checked)
{
    strikeState = checked;
    updateFont();
}

void textDialog::on_checkBoxUnderline_toggled(bool checked)
{
    underlineState = checked;
    updateFont();
}

void textDialog::on_buttonBoxtextDialog_accepted()
{
    //Create font layer
}

void textDialog::on_buttonBoxtextDialog_rejected()
{
    close();
}

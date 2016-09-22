/*

  Text dialog class for the PhotoFiltre LX application.

*/


#include "textdialog.h"
#include "ui_textdialog.h"
#include <QColorDialog>

textDialog::textDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textDialog)
{
    ui->setupUi(this);
    ui->textColour->setAutoFillBackground(true);
    QPalette palette = ui->textColour->palette();
    palette.setColor(ui->textColour->backgroundRole(),Qt::black);
    ui->textColour->setPalette(palette);
    ui->textColour->installEventFilter(this);
    updateFont();
}

textDialog::~textDialog()
{
    delete ui;
}

QColor textDialog::getColorFromLabel(QWidget *colorLabel) const
{
    return colorLabel->palette().color(colorLabel->backgroundRole());
}

void textDialog::showColorDialog(QWidget *colorLabel)
{
    QColor selectedColor = QColorDialog::getColor(getColorFromLabel(colorLabel), this);
    if (selectedColor.isValid()) {
        setColor(selectedColor, colorLabel);
    }
}

bool textDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        if (obj == ui->textColour) {
            showColorDialog(static_cast<QWidget *>(obj));
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void textDialog::setColor(const QColor &color, QWidget *colorLabel)
{
    QPalette palette = colorLabel->palette();
    palette.setColor(colorLabel->backgroundRole(), color);
    colorLabel->setPalette(palette);
    currentColor = color;
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

void textDialog::editText(const QString &text, const QFont &font, const QColor &color)
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
    currentColor = color;

    QPalette palette = ui->textColour->palette();
    palette.setColor(ui->textColour->backgroundRole(),color);
    ui->textColour->setPalette(palette);
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

/*

  Text dialog class for the PhotoFlare application.

*/

#include <QColorDialog>
#include <QSettings>

#include "textdialog.h"
#include "ui_textdialog.h"
#include "../Settings.h"

textDialog::textDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->textColour->setAutoFillBackground(true);
    QPalette palette = ui->textColour->palette();
    palette.setColor(ui->textColour->backgroundRole(),Qt::black);
    ui->textColour->setPalette(palette);
    ui->textColour->installEventFilter(this);
    antialiasEnabled = false;
    ui->plainTextEdit->setFocus();
    ui->emptyWarning->hide();
    ui->alignleftButton->setChecked(true);

    if(SETTINGS->getMemParamsEnabled())
    {
        //Read Dialog settings
        readSettings(this);
    }

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
    if (event->type() == QEvent::MouseButtonRelease)
    {
        if (obj == ui->textColour)
        {
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

void textDialog::on_checkBoxAntialias_toggled(bool checked)
{
    Q_UNUSED(checked);
    antialiasEnabled = true;
}

void textDialog::on_buttonBoxtextDialog_accepted()
{
    currentText = ui->plainTextEdit->toPlainText();
    if(currentText.length() > 0)
    {
        if(SETTINGS->getMemParamsEnabled())
        {
            writeSettings(this);
        }
        accept();
    }
    else
    {
        ui->emptyWarning->show();
    }
}

void textDialog::on_buttonBoxtextDialog_rejected()
{
    close();
}

void textDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        window->resize(settings.value("size").toSize());
        ui->checkBoxBold->setChecked(settings.value("bold").toBool());
        ui->checkBoxItalic->setChecked(settings.value("italic").toBool());
        ui->checkBoxUnderline->setChecked(settings.value("underline").toBool());
        ui->checkBoxStrike->setChecked(settings.value("strikeout").toBool());
        ui->checkBoxAntialias->setChecked(settings.value("antialias").toBool());
        ui->fontSizeSpinner->setValue(settings.value("fontsize").toInt());
        ui->fontComboBox->setCurrentIndex(settings.value("fontselected").toInt());
        ui->textColour->setPalette(settings.value("fontcolour").value<QColor>());
    }
    settings.endGroup();
}

void textDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("size", window->size());
    settings.setValue("bold", ui->checkBoxBold->isChecked() ? "true" : "false");
    settings.setValue("italic", ui->checkBoxItalic->isChecked() ? "true" : "false");
    settings.setValue("underline", ui->checkBoxUnderline->isChecked() ? "true" : "false");
    settings.setValue("strikeout", ui->checkBoxStrike->isChecked() ? "true" : "false");
    settings.setValue("antialias", ui->checkBoxAntialias->isChecked() ? "true" : "false");
    settings.setValue("fontsize", ui->fontSizeSpinner->value());
    settings.setValue("fontselected", ui->fontComboBox->currentIndex());
    settings.setValue("fontcolour", ui->textColour->palette().color(QPalette::Window).name());
    settings.endGroup();
}

void textDialog::on_alignleftButton_clicked()
{

}

void textDialog::on_aligncenterButton_clicked()
{

}

void textDialog::on_alignRightButton_clicked()
{

}

#ifndef PAINTBRUSHSETTINGSWIDGET_H
#define PAINTBRUSHSETTINGSWIDGET_H

/*

  Widget for PaintBrushTool settings.

*/

#include <QWidget>

namespace Ui {
class PaintBrushSettingsWidget;
}

class PaintBrushSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBrushSettingsWidget(QWidget *parent = 0);
    ~PaintBrushSettingsWidget();

    void setAntialiasing(bool value);
    bool antialiasing() const;

    void setBrushWidth(int width);
    int brushWidth() const;

    Qt::PenCapStyle brushCapStyle();

signals:
    void settingsChanged();

private slots:
    void on_buttonRoundCap_clicked();

    void on_buttonSquareCap_clicked();

private:
    void setLabelRadiusValue(int value);

    Ui::PaintBrushSettingsWidget *ui;
};

#endif // PAINTBRUSHSETTINGSWIDGET_H

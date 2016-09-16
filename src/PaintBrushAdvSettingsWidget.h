#ifndef PAINTBRUSHADVSETTINGSWIDGET_H
#define PAINTBRUSHADVSETTINGSWIDGET_H

/*

  Widget for paintbrush settings.

*/

#include <QWidget>

namespace Ui {
class PaintBrushAdvSettingsWidget;
}

class PaintBrushAdvSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBrushAdvSettingsWidget(QWidget *parent = 0);
    ~PaintBrushAdvSettingsWidget();

    QPixmap brushPixmap();
    int brushWidth() const;
    int preassure() const;
    bool fade() const;
    int step() const;
signals:
    void settingsChanged();

private slots:

private:

    Ui::PaintBrushAdvSettingsWidget *ui;
};

#endif // PAINTBRUSHADVSETTINGSWIDGET_H

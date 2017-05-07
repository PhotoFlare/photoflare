#ifndef ERASERSETTINGSWIDGET_H
#define ERASERSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class EraserSettingsWidget;
}

class EraserSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EraserSettingsWidget(QWidget *parent = 0);
    ~EraserSettingsWidget();

    int radius() const;

signals:
    void settingsChanged();

private slots:
    void on_radiusSlider_valueChanged(int value);

private:
    Ui::EraserSettingsWidget *ui;
};

#endif // ERASERSETTINGSWIDGET_H

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

private:
    Ui::EraserSettingsWidget *ui;
};

#endif // ERASERSETTINGSWIDGET_H

#ifndef POINTERSETTINGSWIDGET_H
#define POINTERSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class PointerSettingsWidget;
}

class PointerSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointerSettingsWidget(QWidget *parent = 0);
    ~PointerSettingsWidget();

    bool stroke() const;
    bool fill() const;

signals:
    void settingsChanged();

private:
    Ui::PointerSettingsWidget *ui;
};

#endif // POINTERSETTINGSWIDGET_H

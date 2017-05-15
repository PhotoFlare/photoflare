#ifndef SMUDGESETTINGSWIDGET_H
#define SMUDGESETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SmudgeSettingsWidget;
}

class SmudgeSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmudgeSettingsWidget(QWidget *parent = 0);
    ~SmudgeSettingsWidget();

    int radius() const;

signals:
    void settingsChanged();

private:
    Ui::SmudgeSettingsWidget *ui;
};

#endif // SMUDGESETTINGSWIDGET_H

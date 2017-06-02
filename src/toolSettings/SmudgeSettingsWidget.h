#ifndef SMUDGESETTINGSWIDGET_H
#define SMUDGESETTINGSWIDGET_H

/*

  Widget for SmudgeTool settings.

*/

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
    int pressure() const;

signals:
    void settingsChanged();

private slots:

private:
    Ui::SmudgeSettingsWidget *ui;
};

#endif // SMUDGESETTINGSWIDGET_H

#ifndef MAGICWANDSETTINGSWIDGET_H
#define MAGICWANDSETTINGSWIDGET_H

/*

  Widget for MagicWandTool settings.

*/

#include <QWidget>

namespace Ui {
class MagicWandSettingsWidget;
}

class MagicWandSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MagicWandSettingsWidget(QWidget *parent = 0);
    ~MagicWandSettingsWidget();

    int tolerance() const;
    bool color() const;
signals:
    void settingsChanged();

private slots:

private:
    Ui::MagicWandSettingsWidget *ui;
};

#endif // MAGICWANDSETTINGSWIDGET_H

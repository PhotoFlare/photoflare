#ifndef BLURSETTINGSWIDGET_H
#define BLURSETTINGSWIDGET_H

/*

  Widget for BlurTool settings.

*/

#include <QWidget>

namespace Ui {
class BlurSettingsWidget;
}

class BlurSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BlurSettingsWidget(QWidget *parent = 0);
    ~BlurSettingsWidget();

    int radius() const;
    int pressure() const;
signals:
    void settingsChanged();

private slots:
private:
    Ui::BlurSettingsWidget *ui;
};

#endif // BLURSETTINGSWIDGET_H

#ifndef STAMPSETTINGSWIDGET_H
#define STAMPSETTINGSWIDGET_H

/*

  Widget for spray can settings.

*/

#include <QWidget>

namespace Ui {
class StampSettingsWidget;
}

class StampSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StampSettingsWidget(QWidget *parent = 0);
    ~StampSettingsWidget();

    int radius() const;
    int pressure() const;
    int step() const;
    bool fixed() const;
    bool diffuse() const;
signals:
    void settingsChanged();

private slots:
    void onFixedChanged();
private:
    Ui::StampSettingsWidget *ui;
};

#endif // STAMPSETTINGSWIDGET_H

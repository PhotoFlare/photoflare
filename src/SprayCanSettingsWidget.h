#ifndef SPRAYCANSETTINGSWIDGET_H
#define SPRAYCANSETTINGSWIDGET_H

/*

  Widget for spray can settings.

*/

#include <QWidget>

namespace Ui {
class SprayCanSettingsWidget;
}

class SprayCanSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SprayCanSettingsWidget(QWidget *parent = 0);
    ~SprayCanSettingsWidget();

    int radius() const;
    int pressure() const;
    int dispersion() const;
    bool rainbow() const;
signals:
    void settingsChanged();

private slots:

private:
    Ui::SprayCanSettingsWidget *ui;
};

#endif // SPRAYCANSETTINGSWIDGET_H

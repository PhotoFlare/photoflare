#ifndef HUEDIALOG_H
#define HUEDIALOG_H

/*

  Hue dialog class for the PhotoFlare application.

*/

#include <QDialog>

namespace Ui {
class HueDialog;
}

class HueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HueDialog(QWidget *parent, QImage preview);
    ~HueDialog();

    bool eventFilter(QObject *obj, QEvent *event) override;

    void setPreviewImage(const QImage& image);
    QColor color() {return m_color;}
    int degrees() {return m_degrees;}
    bool isColorizeMethod() const;

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_method1_toggled(bool checked);
    void setColor(const QColor &color, QWidget *colorLabel);
    void showColorDialog(QWidget *colorLabel);
    QColor getColorFromLabel(QWidget *colorLabel) const;
    void writeSettings(QWidget* window);
    void readSettings(QWidget* window);

signals:
    void huePreviewChanged(QImage image, bool colorize, QColor color, int degrees);
private:
    Ui::HueDialog *ui;
    QImage m_preview;
    QColor m_color;
    int m_degrees;
};

#endif // HUEDIALOG_H

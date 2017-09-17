#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

/*

  Gradient dialog class for the PhotoFlare application.

*/

#include <QDialog>
#include <brushtypecombobox.h>

enum Direction {N,NE,E,SE,S,SW,W,NW};

namespace Ui {
class GradientDialog;
}

class GradientDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit GradientDialog(QWidget *parent = 0);
    ~GradientDialog();

    Direction direction(){return m_direction;}
    QColor startColor();
    QColor stopColor();

    //QComboBoxEventsInterface methods
    virtual void mousePressEvent(QComboBox* obj, QMouseEvent *e);
private slots:
    void applyDirection();

    void on_buttonBox_accepted();

    void on_startOpacity_valueChanged(int value);

    void on_stopOpacity_valueChanged(int value);

    void on_radioButton_2_toggled(bool checked);

    void writeSettings(QWidget* window);

    void readSettings(QWidget* window);

private:
    Ui::GradientDialog *ui;
    Direction m_direction;
};

#endif // GRADIENTDIALOG_H

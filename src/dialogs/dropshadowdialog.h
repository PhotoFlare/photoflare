#ifndef DROPSHADOWDIALOG_H
#define DROPSHADOWDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <brushtypecombobox.h>
#include <QMouseEvent>

namespace Ui {
class dropshadowDialog;
}

class dropshadowDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit dropshadowDialog(QWidget *parent = 0);
    ~dropshadowDialog();

    void mousePressEvent(QComboBox* obj, QMouseEvent *e);
    QColor color() const;
    int radius() const;
    int offset() const;

private:
    Ui::dropshadowDialog *ui;

};

#endif // DROPSHADOWDIALOG_H

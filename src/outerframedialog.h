#ifndef OUTERFRAMEDIALOG_H
#define OUTERFRAMEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <brushtypecombobox.h>
#include <QMouseEvent>

namespace Ui {
class OuterFrameDialog;
}

class OuterFrameDialog : public QDialog, QComboBoxEventsInterface
{
    Q_OBJECT

public:
    explicit OuterFrameDialog(QWidget *parent = 0);
    ~OuterFrameDialog();

    void mousePressEvent(QComboBox* obj, QMouseEvent *e);
    QColor color() const;
    int width() const;
private:
    Ui::OuterFrameDialog *ui;
};

#endif // OUTERFRAMEDIALOG_H

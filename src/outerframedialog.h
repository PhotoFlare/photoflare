#ifndef OUTERFRAMEDIALOG_H
#define OUTERFRAMEDIALOG_H

#include <QDialog>

namespace Ui {
class OuterFrameDialog;
}

class OuterFrameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OuterFrameDialog(QWidget *parent = 0);
    ~OuterFrameDialog();

private:
    Ui::OuterFrameDialog *ui;
};

#endif // OUTERFRAMEDIALOG_H

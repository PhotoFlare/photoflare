#ifndef DROPSHADOWDIALOG_H
#define DROPSHADOWDIALOG_H

#include <QDialog>

namespace Ui {
class dropshadowDialog;
}

class dropshadowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dropshadowDialog(QWidget *parent = 0);
    ~dropshadowDialog();

private:
    Ui::dropshadowDialog *ui;
};

#endif // DROPSHADOWDIALOG_H

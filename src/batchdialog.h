#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>

namespace Ui {
class batchDialog;
}

class batchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit batchDialog(QWidget *parent = 0);
    ~batchDialog();

private:
    Ui::batchDialog *ui;
};

#endif // BATCHDIALOG_H

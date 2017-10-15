#ifndef CHECKUPDATEDIALOG_H
#define CHECKUPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class checkupdateDialog;
}

class checkupdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit checkupdateDialog(QWidget *parent = 0);
    ~checkupdateDialog();

private:
    Ui::checkupdateDialog *ui;
};

#endif // CHECKUPDATEDIALOG_H

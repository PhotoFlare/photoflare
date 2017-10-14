#ifndef COLOURMANAGERDIALOG_H
#define COLOURMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class colourManagerDialog;
}

class colourManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit colourManagerDialog(QWidget *parent = 0);
    ~colourManagerDialog();

private:
    Ui::colourManagerDialog *ui;
};

#endif // COLOURMANAGERDIALOG_H

#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include "ui_mainwindow.h"

namespace Ui {
class newDialog;
}

class newDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newDialog(QWidget *parent = 0, Ui::MainWindow* window = 0);
    ~newDialog();

private slots:
    void on_buttonBox_accepted();

    void on_imagePresetCombo_currentIndexChanged(int index);

private:
    Ui::newDialog *ui;
    Ui::MainWindow *uimain;
};

#endif // NEWDIALOG_H

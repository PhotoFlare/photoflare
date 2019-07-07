/*

  About dialog class for the PhotoFlare application.

*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = 0);
    ~aboutDialog();

private slots:
    void on_closeButton_clicked();
    void on_creditsButton_clicked();

private:
    Ui::aboutDialog *ui;
};

#endif // ABOUTDIALOG_H

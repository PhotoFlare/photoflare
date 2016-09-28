#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

/*

  Preferences dialog class.

*/

#include <QDialog>

namespace Ui {
class prefsDialog;
}

class prefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit prefsDialog(QWidget *parent = 0);
    ~prefsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_openFolderButton_clicked();

    void on_saveFolderButton_clicked();

private:
    Ui::prefsDialog *ui;
};

#endif // PREFSDIALOG_H

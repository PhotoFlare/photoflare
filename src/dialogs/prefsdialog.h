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

    void on_compressionSlider_sliderMoved(int position);

    void on_historySlider_sliderMoved(int position);

    void on_restartButton_clicked();

    void on_comboBoxLanguage_currentIndexChanged(const QString &arg1);

private:
    Ui::prefsDialog *ui;
    QString flagPath;
};

#endif // PREFSDIALOG_H

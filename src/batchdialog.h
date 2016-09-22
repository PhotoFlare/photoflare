#ifndef BATCHDIALOG_H
#define BATCHDIALOG_H

#include <QDialog>

class BatchDialogPrivate;

namespace Ui {
class batchDialog;
}

class batchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit batchDialog(QWidget *parent = 0);
    ~batchDialog();

    QStringList fileList();
    QStringList filterList();
    QString outDir();
    void onFinished();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_listWidget_2_doubleClicked(const QModelIndex &index);

    void on_listWidget_3_doubleClicked(const QModelIndex &index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::batchDialog *ui;
    BatchDialogPrivate *d;
};

#endif // BATCHDIALOG_H

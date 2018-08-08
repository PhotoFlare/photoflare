#ifndef BATCHPREGRESS_H
#define BATCHPREGRESS_H

#include <QDialog>

namespace Ui {
class BatchProgress;
}

class BatchProgress : public QDialog
{
    Q_OBJECT

public:
    explicit BatchProgress(QWidget *parent = 0);
    ~BatchProgress();

public slots:
    void progress(int,int);

private slots:
    void on_hideButton_clicked();

private:
    Ui::BatchProgress *ui;
};

#endif // BATCHPROGRESS_H

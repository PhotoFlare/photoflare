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

private:
    Ui::BatchProgress *ui;
};

#endif // BATCHPREGRESS_H

#ifndef BATCHPREGRESS_H
#define BATCHPREGRESS_H

#include <QDialog>

namespace Ui {
class BatchPregress;
}

class BatchPregress : public QDialog
{
    Q_OBJECT

public:
    explicit BatchPregress(QWidget *parent = 0);
    ~BatchPregress();

public slots:
    void progress(int,int);

private:
    Ui::BatchPregress *ui;
};

#endif // BATCHPREGRESS_H

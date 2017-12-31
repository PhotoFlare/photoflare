#ifndef BATCHPROCESSWORKER_H
#define BATCHPROCESSWORKER_H

#include <QObject>

#include "batchdialog.h"
#include "../mainwindow.h"

class BatchProcessWorker : public QObject
{
    Q_OBJECT
public:
    explicit BatchProcessWorker(QObject *parent = 0);

    void setParams(batchDialog* params){m_params = params;}
    void setParent(MainWindow* parent){m_parent = parent;}
public slots:
    void process();

signals:

    void fileProcessFinished(QString file, QImage image);
    void batchProgress(int idx, int total);
private:
    batchDialog* m_params;
    MainWindow* m_parent;
};

#endif // BATCHPROCESSWORKER_H

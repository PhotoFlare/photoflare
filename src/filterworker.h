#ifndef FILTERWORKER_H
#define FILTERWORKER_H

#include <QObject>
#include "mainwindow.h"

class FilterWorker : public QObject
{
    Q_OBJECT
public:
    //FilterWorker();
    explicit FilterWorker(QObject *parent = 0);
    void setParent(MainWindow* parent){m_parent = parent;}
    void setImage(QImage image);

public slots:
    void process();

signals:
    void fileProcessFinished(QImage image);

private:
    MainWindow* m_parent;
    QImage currentImage;
};

#endif // FILTERWORKER_H

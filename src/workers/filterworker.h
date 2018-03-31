#ifndef FILTERWORKER_H
#define FILTERWORKER_H

#include <QObject>
#include "../mainwindow.h"

class FilterWorker : public QObject
{
    Q_OBJECT
public:
    //FilterWorker();
    explicit FilterWorker(QObject *parent = 0);
    void setParent(MainWindow* parent){m_parent = parent;}
    void setImage(QImage image);
    void setFilter(QString filter);
    void setDoubleVal(double v);

public slots:
    void process();

signals:
    void filterProcessFinished(QImage image);

private:
    MainWindow* m_parent;
    QImage currentImage;
    QString currentFilter;
    double currentDouble;
};

#endif // FILTERWORKER_H

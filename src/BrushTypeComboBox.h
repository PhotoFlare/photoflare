#ifndef BRUSHTYPECOMBOBOX_H
#define BRUSHTYPECOMBOBOX_H

#include <QComboBox>

class QComboBoxEventsInterface
{
public:
    virtual void mousePressEvent(QMouseEvent *e) = 0;
};

class BrushTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit BrushTypeComboBox(QWidget *parent = 0);
    ~BrushTypeComboBox();

    void setOnClickHandler(QComboBoxEventsInterface *handler);
signals:

public slots:

protected:
    void paintEvent(QPaintEvent* pEvent);
    void mousePressEvent(QMouseEvent *e);
private:
    QComboBoxEventsInterface *onClickHandler;
};

#endif // BRUSHTYPECOMBOBOX_H

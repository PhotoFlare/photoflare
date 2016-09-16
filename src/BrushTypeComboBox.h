#ifndef BRUSHTYPECOMBOBOX_H
#define BRUSHTYPECOMBOBOX_H

#include <QComboBox>

class BrushTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit BrushTypeComboBox(QWidget *parent = 0);
    ~BrushTypeComboBox();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* pEvent);
};

#endif // BRUSHTYPECOMBOBOX_H

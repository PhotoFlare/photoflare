#ifndef COLOURPICKERTOOL_H
#define COLOURPICKERTOOL_H

#include "Tool.h"

class ColourPickerToolPrivate;
class MainWindow;

class ColourPickerTool : public Tool
{
public:
    ColourPickerTool(QObject *parent = 0);
    ~ColourPickerTool();

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;

signals:

public slots:

private:
    ColourPickerToolPrivate *d;
    MainWindow *main;

};

#endif // COLOURPICKERTOOL_H

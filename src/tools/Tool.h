#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>

class QPaintDevice;

/*!
 * This is the base class for all paint tools
 */
class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject *parent = 0);
    ~Tool();

    void setPaintDevice(QPaintDevice *paintDevice);
    QPaintDevice* paintDevice() const;

    void setScale(float scale);
    /*!
     * Use methods below to paint using painter()
     */
    virtual void onMousePress(const QPoint &pos, Qt::MouseButton button);
    virtual void onMouseMove(const QPoint &pos);
    virtual void onMouseRelease(const QPoint &pos);
    virtual void onKeyPressed(QKeyEvent *keyEvent);
    virtual void onKeyReleased(QKeyEvent *keyEvent);

    virtual void disconnect();

    virtual QCursor getCursor();

signals:

    void cursorChanged(QCursor);
    void selectionChanged(QRect);
    /*!
     * This signal must be emitted after the tool completes any paint action
     */
    void painted(QPaintDevice *paintDevice);

    void overlaid(QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode);

protected:
    QPaintDevice *m_paintDevice;
    float m_scale;
};



#endif // TOOL_H


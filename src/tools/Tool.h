#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QCursor>

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

    /*!
     * Use methods below to paint using painter()
     */
    virtual void onMousePress(const QPoint &pos, Qt::MouseButton button);
    virtual void onMouseMove(const QPoint &pos);
    virtual void onMouseRelease(const QPoint &pos);

    virtual void disconnect();

    virtual QCursor getCursor();

signals:
    /*!
     * This signal must be emitted after the tool completes any paint action
     */
    void painted(QPaintDevice *paintDevice);

    void overlaid(QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode);

protected:
    QPaintDevice *m_paintDevice;
};



#endif // TOOL_H


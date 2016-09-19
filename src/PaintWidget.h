#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

/*

  Paint widget class. Handles the drawing area.

*/

#include <QGraphicsView>

class PaintWidgetPrivate;
class QProgressIndicator;
class Tool;

class PaintWidget : public QGraphicsView
{
    friend class PaintWidgetPrivate;
    Q_OBJECT
public:
    /*!
     * Constructs the widget that contains an image loaded from imagePath
     */
    PaintWidget(const QString &imagePath, QWidget *parent = 0);
    /*!
     * Constructs the widget that contains an empty image with passed size
     */
    PaintWidget(const QSize &imageSize, QWidget *parent = 0);
    ~PaintWidget();

    void setPaintTool(Tool *tool);

    void setImage(const QImage &image);
    QImage image() const;

    QString imagePath() const;

    void autoScale();
    void setScale(const QString &scale);
    float getScale();

    void undo();
    void redo();

    bool isUndoEnabled();
    bool isRedoEnabled();

    void showProgressIndicator(bool visible);
signals:
    // Temporary signal to inform UI that user painted something.
    void contentChanged();

    void zoomChanged(float scale);

protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    PaintWidgetPrivate *d;
    QList<QImage> historyList;
    int historyIndex;
    QProgressIndicator *mProgressIndicator;
private slots:
    void onContentChanged();
    void init();
};

#endif // PAINTWIDGET_H

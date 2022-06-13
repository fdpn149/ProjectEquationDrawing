#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "graphicsscene.h"

class GraphicsView : public QGraphicsView
{
    QPoint lastPosition;
    bool isMoving = false;

    int lastDeltaX = 0;
    int lastDeltaY = 0;
public:
    GraphicsView(QWidget* parent = 0);
    ~GraphicsView();
    GraphicsScene* scene;
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // GRAPHICSVIEW_H

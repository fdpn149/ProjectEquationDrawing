#include "graphicsview.h"
#include "manager.h"
#include <iostream>
#include <cstdlib>

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    scene = new GraphicsScene(this, 521, 521);
    this->setScene(scene);
}

GraphicsView::~GraphicsView()
{
    delete scene;
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastPosition = event->position().toPoint();
        isMoving = true;
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    isMoving = false;
    lastDeltaX = 0;
    lastDeltaY = 0;
}


void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (isMoving)
    {
        const int deltaX = event->position().x() - lastPosition.x();
        const int deltaY = event->position().y() - lastPosition.y();

        scene->moveScene(deltaX-lastDeltaX, deltaY-lastDeltaY);
        manager.showGraph();  //顯示函數圖形
        lastDeltaX = deltaX;
        lastDeltaY = deltaY;
    }
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    double factor;  //縮放因數
    if (event->angleDelta().y() < 0)  //縮小
        factor = 0.5;
    else  //放大
        factor = 2;

    QPointF now_pos = event->position();  //現在滑鼠在畫面上的位置

    scene->zoomScene(now_pos, factor);  //縮放畫面
    manager.showGraph();  //顯示函數圖形
}

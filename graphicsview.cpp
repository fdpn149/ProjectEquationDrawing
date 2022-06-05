#include "graphicsview.h"
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

        system("cls");
        std::cout << deltaX << std::endl << deltaY << std::endl;
        std::cout << deltaX-lastDeltaX << std::endl << deltaY-lastDeltaY << std::endl;

        scene->moveScene(deltaX-lastDeltaX, deltaY-lastDeltaY);

        lastDeltaX = deltaX;
        lastDeltaY = deltaY;
    }
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    double factor = 1.1;

    if (event->angleDelta().y() < 0)
        factor = 0.9;

    QPointF view_pos = event->position();
    QPointF scene_pos = this->mapToScene(view_pos.toPoint());

    system("cls");
    std::cout << "viewPos=  " << view_pos.x() << "  " << view_pos.y() << std::endl;
    std::cout << "scenePos=  " << scene_pos.x() << "  " << scene_pos.y() << std::endl;

    this->centerOn(scene_pos);
    this->scale(factor, factor);

    QPointF delta = this->mapToScene(view_pos.toPoint()) - this->mapToScene(this->viewport()->rect().center());

    this->centerOn(scene_pos - delta);

}

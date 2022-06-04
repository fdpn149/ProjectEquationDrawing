#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsLineItem>
#include <vector>
#include <deque>

using std::vector;
using std::deque;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsScene(QObject *parent, int width, int height);
    ~GraphicsScene();
    double scaleValue = 1;
    double x_min = -10;
    double x_max = 10;
    double y_min = -10;
    double y_max = 10;
    vector<double> create_data(double start, double end, int segment_count);
    void moveScene(int x, int y);
    deque<QGraphicsLineItem*> grid_h;  //水平格線
    deque<QGraphicsLineItem*> grid_v;  //垂直格線
    vector<QPainterPath*> func;  //函數
    void draw();
private:
    const int VIEW_WIDTH;  //介面寬度
    const int VIEW_HEIGHT;  //介面高度
    const int CENTER_X;  //中心的X坐標
    const int CENTER_Y;  //中心的Y坐標
    int xAxisPos = 5;
    int yAxisPos = 5;
};

#endif//GRAPHICSSCENE_H

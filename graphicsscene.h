#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsTextItem>
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
    int xGridMin = -5;  //x格線的最小值
    int xGridMax = 4;  //x格線的最大值
    int yGridMin = -4;  //y格線的最小值
    int yGridMax = 5;  //y格線的最大值
    double x_min = -5;  //x坐標在顯示範圍的最小值
    double x_max = 5;  //x坐標在顯示範圍的最大值
    double y_min = -5;  //y坐標在顯示範圍的最小值
    double y_max = 5;  //y坐標在顯示範圍最大值
    double o_x;  //xy坐標(0,0)的畫面x坐標
    double o_y;  //xy坐標(0,0)的畫面y坐標
    vector<double> calculateGraph(double first, double last, int count, int index);  //計算出函數對應的y，存到vector
    double to_view_y(double value);  //轉換xy坐標成畫面坐標
    void moveScene(int x, int y);
    void zoomScene(QPointF point, double scale);
    deque<QGraphicsLineItem*> grid_h;  //水平格線
    deque<QGraphicsLineItem*> grid_v;  //垂直格線
    deque<QGraphicsTextItem*> text_x;  //x軸數字
    deque<QGraphicsTextItem*> text_y;  //y軸數字
    void draw();
    void removeGraph(int index);
private:
    const int VIEW_WIDTH;  //介面寬度
    const int VIEW_HEIGHT;  //介面高度
    const int CENTER_X;  //中心的X坐標
    const int CENTER_Y;  //中心的Y坐標
    const double PRECISION = 500; //描點準確度
    const double INF = std::numeric_limits<double>::infinity();  //定義INF為double的無限大
};

#endif//GRAPHICSSCENE_H

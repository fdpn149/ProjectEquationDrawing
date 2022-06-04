#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject* parent, int width, int height)
    : QGraphicsScene(parent), VIEW_WIDTH(width), VIEW_HEIGHT(height), CENTER_X(width / 2.0), CENTER_Y(height / 2.0)
{
    grid_h.resize(10);  //設定初始大小為10
    grid_v.resize(10);  //設定初始大小為10

    QPen pen;
    pen.setWidth(3);

    QGraphicsLineItem* xAxis = new QGraphicsLineItem();
    xAxis->setPen(pen);
    xAxis->setLine(0, CENTER_Y, width, CENTER_Y);
    this->addItem(xAxis);
    grid_h.at(5) = xAxis;

    QGraphicsLineItem* yAxis = new QGraphicsLineItem();
    yAxis->setPen(pen);
    yAxis->setLine(CENTER_X, 0, CENTER_X, height);
    this->addItem(yAxis);
    grid_v.at(5) = yAxis;

    pen.setWidth(1);
    for (int i = 0; i < 10; i++)
    {
        if (i == 5)  //跳過坐標軸的線
            continue;
        QGraphicsLineItem* hGrid = new QGraphicsLineItem();
        hGrid->setPen(pen);
        hGrid->setLine(0, i * height / 10, width, i * height / 10);
        this->addItem(hGrid);
        grid_h.at(i) = hGrid;

        QGraphicsLineItem* vGrid = new QGraphicsLineItem();
        vGrid->setPen(pen);
        vGrid->setLine(i * width / 10, 0, i * width / 10, height);
        this->addItem(vGrid);
        grid_v.at(i) = vGrid;
    }

    draw();  //畫正弦波
}

GraphicsScene::~GraphicsScene()
{

}

vector<double> GraphicsScene::create_data(double start, double end, int segment_count)
{
    double delta = (end - start) / (double)segment_count;
    size_t point_count = segment_count + 1;

    vector<double> data;

    double x = start;

    for (size_t i = 0; i < point_count; ++i)
    {
        data.push_back(std::sin(x));

        x += delta;
    }

    return data;
}

void GraphicsScene::moveScene(int x, int y)
{
    int xRange = 10;
    int yRange = 10;
    QGraphicsLineItem* nowItem;
    qreal x1, y1, x2, y2;

    //垂直移動
    for (int i = 0; i < yRange; i++)
    {
        nowItem = grid_h.at(i);
        this->removeItem(nowItem);
        x1 = nowItem->line().x1();
        y1 = nowItem->line().y1() + y;
        x2 = nowItem->line().x2();
        if (y1 < 0)
        {
            y1 = VIEW_HEIGHT + y1;
            grid_h.pop_front();
            i--;
            yRange--;
            yAxisPos--;
            nowItem->setLine(x1, y1, x2, y1);
            this->addItem(nowItem);
            grid_h.push_back(nowItem);
        }
        else
        {
            nowItem->setLine(x1, y1, x2, y1);
            this->addItem(nowItem);
        }
    }


    //水平移動
    for (int i = 0; i < 10; i++)
    {
        QGraphicsLineItem* nowItem = grid_h.at(i);
        nowItem = grid_v.at(i);
        this->removeItem(nowItem);
        nowItem->setLine(nowItem->line().x1()+x, nowItem->line().y1(), nowItem->line().x2()+x, nowItem->line().y2());
        this->addItem(nowItem);
    }
}

void GraphicsScene::draw()
{
    vector<double> data = create_data(0.0, 4.0 * M_PI, 200);

    double delta_x = (double)VIEW_WIDTH / 200.0;
    double delta_y = -(double)VIEW_HEIGHT / 5.0;

    double start_x = 0.0;

    QPainterPath path;
    path.moveTo(start_x, data[0] * delta_y + CENTER_Y);

    for (size_t i = 1; i < 201; ++i)
    {
        start_x += delta_x;
        path.lineTo(start_x, data[i] * delta_y + CENTER_Y);
    }

    this->addPath(path);
}

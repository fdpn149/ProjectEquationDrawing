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
	GraphicsScene(QObject* parent, int width, int height);
	~GraphicsScene();
	double scaleValue = 1;	//縮放的比例(無縮放為1,放大<1,縮小>1)
	int x_grid_min = -5;	//x格線的最小值
	int x_grid_max = 4;		//x格線的最大值
	int y_grid_min = -4;	//y格線的最小值
	int y_grid_max = 5;		//y格線的最大值
	double x_min = -5;		//x坐標在顯示範圍的最小值
	double x_max = 5;		//x坐標在顯示範圍的最大值
	double y_min = -5;		//y坐標在顯示範圍的最小值
	double y_max = 5;		//y坐標在顯示範圍最大值
	double o_x;				//xy坐標(0,0)的畫面x坐標
	double o_y;				//xy坐標(0,0)的畫面y坐標
	deque<QGraphicsLineItem*> grid_h;	//水平格線
	deque<QGraphicsLineItem*> grid_v;	//垂直格線
	deque<QGraphicsTextItem*> text_x;	//x軸數字
	deque<QGraphicsTextItem*> text_y;	//y軸數字
	void zoomScene(QPointF point, double scale);  //縮放畫面
	void moveScene(int x, int y);	//移動畫面
	void removeGraph(int index);	//刪除函數圖形
	void draw();					//繪製函數圖形
	double to_view_y(double value);	//轉換xy坐標成畫面坐標
	double to_view_x(double value);	//轉換xy坐標成畫面坐標
	vector<double> calculateGraph(double first, double last, int count, int index, char type);	//計算出函數對應的x或y，存到vector
private:
	const int VIEW_WIDTH;	//介面寬度
	const int VIEW_HEIGHT;	//介面高度
	const int CENTER_X;		//中心的X坐標
	const int CENTER_Y;		//中心的Y坐標
	const double PRECISION = 500;	//描點準確度
	const double INF = std::numeric_limits<double>::infinity();	//定義INF為double的無限大
	QPen axisPen;  //畫坐標軸的筆
	QPen gridPen;  //畫其它格線的筆
	void moveGridForward(deque<QGraphicsLineItem*>& grid, const int& view_max, int& grid_min, int& grid_max,
		const int& line_end, char type, int distance);  //移動格線(向上、向左)
	void moveGridBackward(deque<QGraphicsLineItem*>& grid, const int& view_max, int& grid_min, int& grid_max,
		const int& line_end, char type, int distance);  //移動格線(向下、向右)
	void moveNumberForward(deque<QGraphicsTextItem*>& text, const int& view_max, int x, int y,
		double delta, char type);  //移動數字(向上、向左)
	void moveNumberBackward(deque<QGraphicsTextItem*>& text, const int& view_max, int x, int y,
		double delta, char type);  //移動數字(向下、向右)
	void keepNumber();  //讓數字保持在畫面中
};

#endif//GRAPHICSSCENE_H

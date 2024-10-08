﻿#include "graphicsscene.h"
#include "manager.h"

GraphicsScene::GraphicsScene(QObject* parent, int width, int height)
	: QGraphicsScene(parent), VIEW_WIDTH(width), VIEW_HEIGHT(height), CENTER_X(width / 2.0), CENTER_Y(height / 2.0)
{
	o_x = CENTER_X;  //設定xy坐標原點的x坐標為畫面中央
	o_y = CENTER_Y;  //設定xy坐標原點的y坐標為畫面中央
	axisPen.setWidth(3);  //設定坐標軸之筆的寬度為3

	grid_h.resize(10);  //設定初始大小為10
	grid_v.resize(10);  //設定初始大小為10
	text_x.resize(10);  //設定初始大小為10
	text_y.resize(10);  //設定初始大小為10

	//䢖立x軸
	QGraphicsLineItem* xAxis = new QGraphicsLineItem();
	xAxis->setPen(axisPen);  //設定畫筆為寬度3的筆
	xAxis->setLine(0, CENTER_Y, VIEW_WIDTH, CENTER_Y);  //在中心畫一條橫線
	this->addItem(xAxis);  //將線加到畫面上
	grid_h.at(5) = xAxis;  //將線的物件存到grid_h

	//建立y軸
	QGraphicsLineItem* yAxis = new QGraphicsLineItem();
	yAxis->setPen(axisPen);  //設定畫筆為寬度3的筆
	yAxis->setLine(CENTER_X, 0, CENTER_X, VIEW_HEIGHT);  //在中心畫一條直線
	this->addItem(yAxis);  //將線加到畫面上
	grid_v.at(5) = yAxis;  //將線的物件存到grid_v

	for (int i = 0; i < 10; i++)
	{
		//建立y軸文字
		QGraphicsTextItem* ytext = new QGraphicsTextItem();
		ytext->setPlainText(QString::number(5 - i));  //設定坐標軸上的文字
		ytext->setPos(CENTER_X, i * VIEW_HEIGHT / 10);  //設定文字的位置
		this->addItem(ytext);  //將文字加到畫面上
		text_y.at(i) = ytext;  //將文字存到text_y

		//建立x軸文字
		QGraphicsTextItem* xtext = new QGraphicsTextItem();
		xtext->setPlainText(QString::number(i - 5));  //設定坐標軸上的文字
		xtext->setPos(i * VIEW_WIDTH / 10, CENTER_Y);  //設定文字的位置
		this->addItem(xtext);  //將文字加到畫面上
		text_x.at(i) = xtext;  //將文字存到text_x

		if (i == 5) continue;  //跳過坐標軸的線

		//建立平行於x軸的線
		QGraphicsLineItem* hGrid = new QGraphicsLineItem();
		hGrid->setPen(gridPen);  //設定畫筆為寬度1的筆
		hGrid->setLine(0, i * VIEW_HEIGHT / 10, VIEW_WIDTH, i * VIEW_HEIGHT / 10);  //設定線兩端的位置
		this->addItem(hGrid);  //將線加到畫面上
		grid_h.at(i) = hGrid;  //將線存入grid_h

		QGraphicsLineItem* vGrid = new QGraphicsLineItem();
		vGrid->setPen(gridPen);  //設定畫筆為寬度1的筆
		vGrid->setLine(i * VIEW_WIDTH / 10, 0, i * VIEW_WIDTH / 10, VIEW_HEIGHT);  //設定線兩端的位置
		this->addItem(vGrid);  //將線加到畫面上
		grid_v.at(i) = vGrid;  //將線存入grid_v
	}
}

GraphicsScene::~GraphicsScene()
{
	for (QGraphicsLineItem* q : grid_h)  //刪除水平格線
		delete q;
	for (QGraphicsLineItem* q : grid_v)  //刪除垂直格線
		delete q;
	for (QGraphicsTextItem* q : text_x)  //刪除x軸的數字
		delete q;
	for (QGraphicsTextItem* q : text_y)  //刪除y軸的數字
		delete q;
}

void GraphicsScene::moveScene(int x, int y)
{
	//調整xy的顯示範圍
	x_min -= x * 10.0 / (double)VIEW_WIDTH * scaleValue;
	x_max -= x * 10.0 / (double)VIEW_WIDTH * scaleValue;
	y_min += y * 10.0 / (double)VIEW_HEIGHT * scaleValue;
	y_max += y * 10.0 / (double)VIEW_HEIGHT * scaleValue;

	o_x += x;  //xy原點的畫面x坐標移動
	o_y += y;  //xy原點的畫面y坐標移動

	/*移動格線*/
	//垂直移動
	if (y < 0)  //滑鼠向上拖曳(可視範圍向下增加)
		moveGridForward(grid_h, VIEW_HEIGHT, y_grid_min, y_grid_max, VIEW_WIDTH, 'y', y);
	else  //滑鼠向下拖曳(可視範圍向上增加)
		moveGridBackward(grid_h, VIEW_HEIGHT, y_grid_min, y_grid_max, VIEW_WIDTH, 'y', y);
	//水平移動
	if (x < 0)  //滑鼠向左拖曳(可視範圍向右增加)
		moveGridForward(grid_v, VIEW_WIDTH, x_grid_min, x_grid_max, VIEW_HEIGHT, 'x', x);
	else  //滑鼠向右拖曳(可視範圍向左增加)
		moveGridBackward(grid_v, VIEW_WIDTH, x_grid_min, x_grid_max, VIEW_HEIGHT, 'x', x);

	/*移動數字*/
	//垂直移動
	if (y < 0)  //滑鼠向上拖曳(可視範圍向下增加)
		moveNumberForward(text_y, VIEW_HEIGHT, x, y, -scaleValue, 'y');
	else  //滑鼠向下拖曳(可視範圍向上增加)
		moveNumberBackward(text_y, VIEW_HEIGHT, x, y, scaleValue, 'y');
	//水平移動
	if (x < 0)  //滑鼠向左拖曳(可視範圍向右增加)
		moveNumberForward(text_x, VIEW_WIDTH, x, y, scaleValue, 'x');
	else  //滑鼠向右拖曳(可視範圍向左增加)
		moveNumberBackward(text_x, VIEW_WIDTH, x, y, -scaleValue, 'x');

	/*超出邊界時需吸付在邊邊*/
	keepNumber();
}

void GraphicsScene::zoomScene(QPointF point, double scale)
{
	if (scaleValue <= 0.0625 && scale > 1) return;  //若放太大就不可再繼續放大

	const double zoom_x = point.x();  //滑鼠(縮放點)的畫面x坐標
	const double zoom_y = point.y();  //滑鼠(縮放點)的畫面y坐標
	scaleValue /= scale;  //更改總共的縮放倍率
	x_min /= scale;  //x最小值調整
	x_max /= scale;  //x最大值調整
	y_min /= scale;  //y最小值調整
	y_max /= scale;  //y最大值調整
	for (int i = 0; i < 10; i++)
	{
		text_x.at(i)->setPlainText(QString::number(text_x.at(i)->toPlainText().toDouble() / scale));  //調整目前畫面上的x軸數字
		text_y.at(i)->setPlainText(QString::number(text_y.at(i)->toPlainText().toDouble() / scale));  //調整目前畫面上的y軸數字
	}
	if (scale > 1)  //若為放大
	{
		double dx = o_x - zoom_x;  //原點與縮放點的x距離差
		if (dx > 0)  //若距離差為正
			for (; dx > 500; dx -= 500)  //以500為單位移動畫面
				moveScene(500, 0);
		else  //若距離不為正
			for (; dx < -500; dx += 500)  //以-500為單位移動畫面
				moveScene(-500, 0);
		moveScene(dx, 0);  //移動最後不到500的距離

		double dy = o_y - zoom_y;  //原點與縮放點的y距離差
		if (dy > 0)  //若距離差為正
			for (; dy > 500; dy -= 500)  //以500為單位移動畫面
				moveScene(0, 500);
		else  //若距離不為正
			for (; dy < -500; dy += 500)  //以-500為單位移動畫面
				moveScene(0, -500);
		moveScene(0, dy);  //移動最後不到500的距離
	}
	else  //若為縮小
	{
		double dx = (zoom_x - o_x) / 2;  //原點與縮放點的x距離差÷(-2)
		if (dx > 0)  //若距離差為正
			for (; dx > 500; dx -= 500)  //以500為單位移動畫面
				moveScene(500, 0);
		else
			for (; dx < -500; dx += 500)  //以-500為單位移動畫面
				moveScene(-500, 0);
		moveScene(dx, 0);  //移動最後不到500的距離

		double dy = (zoom_y - o_y) / 2;  //原點與縮放點的y距離差÷(-2)
		if (dy > 0)  //若距離差為正
			for (; dy > 500; dy -= 500)  //以500為單位移動畫面
				moveScene(0, 500);
		else
			for (; dy < -500; dy += 500)  //以-500為單位移動畫面
				moveScene(0, -500);
		moveScene(0, dy);  //移動最後不到500的距離
	}
}

void GraphicsScene::moveGridForward(deque<QGraphicsLineItem*>& grid, const int& view_max, int& grid_min, int& grid_max, const int& line_end, char type, int distance)
{
	int range = 10;  //設定迴圈範圍的初始值為10
	double new_xy;  //新的x或y
	QGraphicsLineItem* nowItem;  //儲存現在的線物件(LineItem)
	for (int i = 0; i < range; i++)
	{
		nowItem = grid.at(i);  //現在物件設為第i個線物件
		if (type == 'y')  //若是橫線
			new_xy = nowItem->line().y1() + distance;  //設定new_xy為移動後的y坐標
		else  //若是直線
			new_xy = nowItem->line().x1() + distance;  //設定new_xy為移動後的x坐標

		if (new_xy < 0)  //若新的x或y坐標超出畫面範圍
		{
			new_xy = view_max + new_xy;  //將線移到畫面另一側，並補上超出去的部分
			if (type == 'y')  //若是橫線
			{
				grid_min--;  //格線的最小值減1
				grid_max--;  //格線的最大值減1
				if (grid_min == 0)  //如果是第0條(x軸)
					nowItem->setPen(axisPen);  //設定為坐標軸的筆
				else
					nowItem->setPen(gridPen);  //設定為一般的筆
			}
			else  //若是直線
			{
				grid_min++;  //格線的最小值加1
				grid_max++;  //格線的最大值加1
				if (grid_max == 0)  //如果是第0條(y軸)
					nowItem->setPen(axisPen);  //設定為坐標軸的筆
				else
					nowItem->setPen(gridPen);  //設定為一般的筆
			}
			grid.pop_front();  //將原本儲存在grid的nowItem清除(此時的nowItem為第0項)
			grid.push_back(nowItem);  //將nowItem放到最後一項
			i--;  //i減1(因為grid的順序改變，所以i要減1與i++抵消)
			range--;  //迴圈執行範圍縮減(因為grid的順序有改變，執行到原來的range會重複執行到)
		}

		if (type == 'y')  //若是橫線
			nowItem->setLine(0, new_xy, line_end, new_xy);  //設定線兩端的位置
		else  //若是直線
			nowItem->setLine(new_xy, 0, new_xy, line_end);  //設定線兩端的位置
	}
}

void GraphicsScene::moveGridBackward(deque<QGraphicsLineItem*>& grid, const int& view_max, int& grid_min, int& grid_max, const int& line_end, char type, int distance)
{
	int range = 0;  //設定迴圈範圍的初始值為0
	double new_xy;  //新的x或y
	QGraphicsLineItem* nowItem;  //儲存現在的線物件(LineItem)
	for (int i = 9; i >= range; i--)
	{
		nowItem = grid.at(i);  //現在物件設為第i個線物件
		if (type == 'y')  //若是橫線
			new_xy = nowItem->line().y1() + distance;  //設定new_xy為移動後的y坐標
		else  //若是直線
			new_xy = nowItem->line().x1() + distance;  //設定new_xy為移動後的x坐標

		if (new_xy > view_max)  //若新的x或y坐標超出畫面範圍
		{
			new_xy = new_xy - view_max;  //將線移到畫面另一側，並補上超出去的部分
			if (type == 'y')  //若是橫線
			{
				grid_min++;  //格線的最小值加1
				grid_max++;  //格線的最大值加1
				if (grid_max == 0)  //如果是第0條(x軸)
					nowItem->setPen(axisPen);  //設定為坐標軸的筆
				else
					nowItem->setPen(gridPen);  //設定為一般的筆
			}
			else  //若是直線
			{
				grid_min--;  //格線的最小值減1
				grid_max--;  //格線的最大值減1
				if (grid_min == 0)  //如果是第0條(y軸)
					nowItem->setPen(axisPen);  //設定為坐標軸的筆
				else
					nowItem->setPen(gridPen);  //設定為一般的筆
			}
			grid.pop_back();  //將原本儲存在grid的nowItem清除(此時的nowItem為最後一項)
			grid.push_front(nowItem);  //將nowItem放到第0項
			i++;  //i減1(因為grid的順序改變，所以i要加1與i--抵消)
			range++;  //迴圈執行範圍縮減(因為grid的順序有改變，執行到原來的range會少執行到)
		}

		if (type == 'y')  //若是橫線
			nowItem->setLine(0, new_xy, line_end, new_xy);  //設定線兩端的位置
		else  //若是直線
			nowItem->setLine(new_xy, 0, new_xy, line_end);  //設定線兩端的位置
	}
}

void GraphicsScene::moveNumberForward(deque<QGraphicsTextItem*>& text, const int& view_max, int x, int y, double delta, char type)
{
	int range = 10;  //設定迴圈範圍的初始值為10
	double new_x, new_y;  //新的x,新的y
	QGraphicsTextItem* nowTextItem;  //儲存現在的文字物件
	for (int i = 0; i < range; i++)
	{
		nowTextItem = text.at(i);  //現在物件設為第i個文字物件
		new_x = nowTextItem->pos().x() + x;  //設定新的x為移動後的x
		new_y = nowTextItem->pos().y() + y;  //設定新的y為移動後的y
		if ((type == 'x' && new_x < 0) || (type == 'y' && new_y < 0))  //若新的坐標小於0
		{
			if (type == 'x')	new_x = view_max + new_x;  //移到畫面另一側，並補上超出去的部分
			else				new_y = view_max + new_y;  //移到畫面另一側，並補上超出去的部分
			text.pop_front();  //將text的第0項刪掉
			i--;  //i減1(因為grid的順序改變，所以i要減1與i++抵消)
			range--;  //迴圈執行範圍縮減(因為text的順序有改變，執行到原來的range會重複執行到)
			nowTextItem->setPos(new_x, new_y);  //設定文字的位置
			nowTextItem->setPlainText(QString::number(text.at(text.size() - 1)->toPlainText().toDouble() + delta));  //設定文字為移動過的數字
			text.push_back(nowTextItem);  //將修改後的文字物件放到最後一項
		}
		else
		{
			nowTextItem->setPos(new_x, new_y);  //設定文字的位置
		}
	}
}

void GraphicsScene::moveNumberBackward(deque<QGraphicsTextItem*>& text, const int& view_max, int x, int y, double delta, char type)
{
	int range = 0;  //設定迴圈範圍的初始值為0
	double new_x, new_y;  //新的x,新的y
	QGraphicsTextItem* nowTextItem;  //儲存現在的文字物件
	for (int i = 9; i >= range; i--)
	{
		nowTextItem = text.at(i);  //現在物件設為第i個文字物件
		new_x = nowTextItem->pos().x() + x;  //設定新的x為移動後的x
		new_y = nowTextItem->pos().y() + y;  //設定新的y為移動後的y
		if ((type == 'x' && new_x > view_max) || (type == 'y' && new_y > view_max))  //若新的坐標大於顯示範圍最大值
		{
			if (type == 'x')	new_x = new_x - view_max;  //移到畫面另一側，並減掉多出去的部分
			else				new_y = new_y - view_max;  //移到畫面另一側，並減掉多出去的部分
			text.pop_back();  //將text的最後一項刪掉
			i++;  //i減1(因為text的順序改變，所以i要加1與i--抵消)
			range++;  //迴圈執行範圍縮減(因為text的順序有改變，執行到原來的range會少執行到)
			nowTextItem->setPos(new_x, new_y);  //設定文字的位置
			nowTextItem->setPlainText(QString::number(text.at(0)->toPlainText().toDouble() + delta));  //設定文字為移動過的數字
			text.push_front(nowTextItem);  //將修改後的文字物件放到最後一項
		}
		else
		{
			nowTextItem->setPos(new_x, new_y);  //設定文字的位置
		}
	}
}

void GraphicsScene::keepNumber()
{
	QGraphicsTextItem* nowTextItem;
	double new_x, new_y;  //新的x,新的y
	//y軸數字判斷
	if (x_grid_max + x_grid_min > 9)  //若y軸超出畫面左邊
	{
		new_x = 0;
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			nowTextItem->setPos(new_x, nowTextItem->pos().y());  //將y軸數字貼在畫面左邊
		}
	}
	else if (x_grid_max + x_grid_min < -9)  //若y軸超出畫面右邊
	{
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			new_x = VIEW_WIDTH - nowTextItem->boundingRect().width();  //x設為畫面寬度減掉文字寬度(靠右對齊)
			nowTextItem->setPos(new_x, nowTextItem->pos().y());  //將y軸數字貼在畫面右邊
		}
	}
	else  //y軸沒超出畫面
	{
		for (int i = 0; i < 10; i++)
		{
			if (grid_v.at(i)->pen() == axisPen)  //找出y軸的位置
			{
				new_x = grid_v.at(i)->line().x1();
				break;
			}
		}
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			nowTextItem->setPos(new_x, nowTextItem->pos().y());  //將y軸數字設在坐標軸邊
		}
	}

	//x軸數字判斷
	if (y_grid_max + y_grid_min < -9)  //若x軸超出畫面上面
	{
		new_y = 0;
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);  //將x軸數字貼在畫面上面
		}
	}
	else if (y_grid_max + y_grid_min > 9)  //若x軸超出畫面下面
	{
		new_y = VIEW_HEIGHT - text_x.at(0)->boundingRect().height();
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);  //將x軸數字貼在畫面下面
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)  //找出x軸的位置
			if (grid_h.at(i)->pen() == axisPen)
			{
				new_y = grid_h.at(i)->line().y1();
				break;
			}
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);  //將x軸數字設在坐標軸邊
		}
	}
}

double GraphicsScene::to_view_y(double value)
{
	if (value == INF || value == -INF)  //若數值為正負無限大
		return value;  //直接回傳
	return (y_max - value) * VIEW_HEIGHT / (y_max - y_min);  //回傳轉換後的數值
}

double GraphicsScene::to_view_x(double value)
{
	if (value == INF || value == -INF)  //若數值為正負無限大
		return value;  //直接回傳
	return (value - x_min) * VIEW_WIDTH / (x_max - x_min);  //回傳轉換後的數值
}

void GraphicsScene::removeGraph(int index)
{
	if (Storage::graphs.at(index)->graph != nullptr)  //若已經有存在的圖形
		this->removeItem(Storage::graphs.at(index)->graph);  //刪除畫面上的圖形
	delete Storage::graphs.at(index)->graph;  //刪除儲存的路徑物件
	Storage::graphs.at(index)->graph = nullptr;  //將原本儲存物件的位置設為null
}

vector<double> GraphicsScene::calculateGraph(double first, double last, int count, int index, char type)
{
	vector<double> data;  //存函數數值
	double dxy = (last - first) / (double)count;  //x或的間距
	int diByZero_count = 0;  //除以零的次數
	double xy = first;  //先將xy設為第1項

	for (int i = 0; i <= count; i++)
	{
		try {
			data.push_back(manager.calculate(xy, type, index));  //計算函數值
		}
		catch (divided_by_zero) {  //若發生除以0
			diByZero_count++;
			data.push_back(INF);
			if (diByZero_count > count)  //如果函數的每一項都是除以0
				throw;
		}
		catch (variable_error) {
			throw;
		}
		catch (std::exception& e) {
			string w = e.what();
			data.push_back(INF);
		}
		xy += dxy;
	}

	return data;
}

void GraphicsScene::draw()
{
	for (int i = 0; i < Storage::graphs.size(); i++)
	{
		try {
			//若找到y=或x=的項，而且狀態為顯示(status==1)
			if (Storage::graphs.at(i)->status == 1 && (Storage::graphs.at(i)->name == "y" || Storage::graphs.at(i)->name == "x"))
			{
				vector<double> data;  //儲存函數值
				double precision;  //精確度(多少間距繪一個點)

				if (scaleValue > 1)  //若縮放倍率大於1
					precision = PRECISION * scaleValue;  //精確度設為縮放倍率÷放大倍率
				else  //若縮放倍率不大於1
					precision = PRECISION;  //精確度設為預設倍率

				QPainterPath path;  //儲存函數路徑

				//繪製y=的函數
				if (Storage::graphs.at(i)->name == "y")
				{
					data = calculateGraph(x_min, x_max, precision, i, 'y');  //計算y的值，存入data

					double dx = (double)VIEW_WIDTH / precision;  //x的間距(畫面坐標)

					double x = 0;  //畫面坐標x初始設為0
					double y = to_view_y(data.at(0));  //畫面坐標y初始設為第0項的畫面坐標
					const double y_display_max = 2000;  //y能顯示的最大值
					const double y_display_min = -2000;  //y能顯示的最小值

					bool need_move = false;  //是否需要移動

					if (y != INF && y != -INF && y <= y_display_max && y >= y_display_min)  //若y不為正負無限大，且y沒有超出顯示範圍太多
						path.moveTo(x, y);  //移動到(x,y)
					else
						need_move = true;  //因為尚未移動，故將need_move設為true

					for (int i = 1; i <= precision; i++)
					{
						x += dx;  //x設為下一個x
						y = to_view_y(data.at(i));  //y設為轉換後的坐標

						if (need_move)  //若需要移動
						{
							if (y != INF && y != -INF && y <= y_display_max && y >= y_display_min)  //若y不為正負無限大，且y沒有超出顯示範圍太多
							{
								path.moveTo(x, y);  //移動到(x,y)
								need_move = false;  //不需要再移動了
							}
						}
						else  //不需移動
						{
							if (y == INF || y == -INF || y > y_display_max || y < y_display_min)  //若y為正負無限大，或y超出顯示範圍太多
								need_move = true;  //需要重新下筆
							else  //y是有效的
								path.lineTo(x, y);  //將線連到(x,y)
						}
					}
				}
				//繪製x=的函數
				else
				{
					data = calculateGraph(y_min, y_max, precision, i, 'x');  //計算x的值，存入data

					double dy = -(double)VIEW_HEIGHT / precision;  //y的間距(畫面坐標)

					double x = to_view_x(data.at(0));  //畫面坐標x初始設為第0項的畫面坐標
					double y = VIEW_HEIGHT;  //畫面坐標y初始設為畫面高度
					const double x_display_max = 2000;  //x能顯示的最大值
					const double x_display_min = -2000;  //x能顯示的最小值

					bool need_move = false;  //是否需要移動

					if (x != INF && x != -INF && x <= x_display_max && x >= x_display_min)  //若x不為正負無限大，且x沒有超出顯示範圍太多
						path.moveTo(x, y);  //移動到(x,y)
					else
						need_move = true;  //因為尚未移動，故將need_move設為true

					for (int i = 1; i <= precision; i++)
					{
						y += dy;  //y設為下一個y
						x = to_view_x(data.at(i));  //x設為轉換後的坐標

						if (need_move)  //若需要移動
						{
							if (x != INF && x != -INF && x <= x_display_max && x >= x_display_min)  //若x不為正負無限大，且x沒有超出顯示範圍太多
							{
								path.moveTo(x, y);  //移動到(x,y)
								need_move = false;  //不需要再移動了
							}
						}
						else  //不需移動
						{
							if (x == INF || x == -INF || x > x_display_max || x < x_display_min)  //若x為正負無限大，或x超出顯示範圍太多
								need_move = true;  //需要重新下筆
							else  //x是有效的
								path.lineTo(x, y);  //將線連到(x,y)
						}
					}
				}
				QGraphicsPathItem* pathItem;  //指向路徑物件的指標
				if (Storage::graphs.at(i)->graph != nullptr)  //若已經有存在的圖形
				{
					pathItem = Storage::graphs.at(i)->graph;  //指向存在的圖形
					this->removeItem(Storage::graphs.at(i)->graph);  //先刪除畫面上的圖形
				}
				else
				{
					pathItem = new QGraphicsPathItem();  //䢖立一個新的物件
					QPen pen;  //建立一支畫筆
					pen.setColor(Storage::graphs.at(i)->color);  //設定顏色為那個函數的顏色
					pen.setWidth(2);  //設定筆刷寬度為2
					pathItem->setPen(pen);  //設定路徑物件的畫筆
				}
				pathItem->setPath(path);  //設定路徑為剛剛畫好的path
				Storage::graphs.at(i)->graph = pathItem;  //儲存圖形
				this->addItem(pathItem);  //在畫面上加上剛剛畫好的圖形
			}
		}
		catch (divided_by_zero) {  //若每一項都為÷0
			manager.removeGraph(i);  //刪除函數
		}
		catch (variable_error) {  //變數有誤
			manager.removeGraph(i);  //刪除函數
		}
	}
}

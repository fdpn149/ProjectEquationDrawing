#include "graphicsscene.h"
#include "manager.h"

GraphicsScene::GraphicsScene(QObject* parent, int width, int height)
	: QGraphicsScene(parent), VIEW_WIDTH(width), VIEW_HEIGHT(height), CENTER_X(width / 2.0), CENTER_Y(height / 2.0)
{
	o_x = CENTER_X;
	o_y = CENTER_Y;

	grid_h.resize(10);  //設定初始大小為10
	grid_v.resize(10);  //設定初始大小為10
	text_x.resize(10);  //設定初始大小為10
	text_y.resize(10);  //設定初始大小為10

	QPen pen;
	pen.setWidth(3);

	//䢖立x軸
	QGraphicsLineItem* xAxis = new QGraphicsLineItem();
	xAxis->setPen(pen);
	xAxis->setLine(0, CENTER_Y, VIEW_WIDTH, CENTER_Y);
	this->addItem(xAxis);
	grid_h.at(5) = xAxis;

	//建立y軸
	QGraphicsLineItem* yAxis = new QGraphicsLineItem();
	yAxis->setPen(pen);
	yAxis->setLine(CENTER_X, 0, CENTER_X, VIEW_HEIGHT);
	this->addItem(yAxis);
	grid_v.at(5) = yAxis;

	pen.setWidth(1);
	for (int i = 0; i < 10; i++)
	{
		//建立y軸文字
		QGraphicsTextItem* ytext = new QGraphicsTextItem();
		ytext->setPlainText(QString::number(5 - i));
		ytext->setPos(CENTER_X, i * VIEW_HEIGHT / 10);
		this->addItem(ytext);
		text_y.at(i) = ytext;

		//建立x軸文字
		QGraphicsTextItem* xtext = new QGraphicsTextItem();
		xtext->setPlainText(QString::number(i - 5));
		xtext->setPos(i * VIEW_WIDTH / 10, CENTER_Y);
		this->addItem(xtext);
		text_x.at(i) = xtext;

		if (i == 5) continue;  //跳過坐標軸的線

		QGraphicsLineItem* hGrid = new QGraphicsLineItem();
		hGrid->setPen(pen);
		hGrid->setLine(0, i * VIEW_HEIGHT / 10, VIEW_WIDTH, i * VIEW_HEIGHT / 10);
		this->addItem(hGrid);
		grid_h.at(i) = hGrid;

		QGraphicsLineItem* vGrid = new QGraphicsLineItem();
		vGrid->setPen(pen);
		vGrid->setLine(i * VIEW_WIDTH / 10, 0, i * VIEW_WIDTH / 10, VIEW_HEIGHT);
		this->addItem(vGrid);
		grid_v.at(i) = vGrid;
	}

	//draw();  //畫正弦波
}

GraphicsScene::~GraphicsScene()
{
	for (QGraphicsLineItem* q : grid_h)
		delete q;
	for (QGraphicsLineItem* q : grid_v)
		delete q;
	for (QGraphicsTextItem* q : text_x)
		delete q;
	for (QGraphicsTextItem* q : text_y)
		delete q;
}

void GraphicsScene::moveScene(int x, int y)
{
	double new_x, new_y;
	QPen axisPen;
	QPen gridPen;
	axisPen.setWidth(3);
	int range;

	//調整xy的顯示範圍
	x_min -= x * 10.0 / (double)VIEW_WIDTH * scaleValue;
	x_max -= x * 10.0 / (double)VIEW_WIDTH * scaleValue;
	y_min += y * 10.0 / (double)VIEW_HEIGHT * scaleValue;
	y_max += y * 10.0 / (double)VIEW_HEIGHT * scaleValue;

	o_x += x;
	o_y += y;

	//移動格線//
	QGraphicsLineItem* nowItem;
	//垂直移動
	if (y < 0)  //滑鼠向上拖曳(可視範圍向下增加)
	{
		range = 10;
		for (int i = 0; i < range; i++)
		{
			nowItem = grid_h.at(i);
			this->removeItem(nowItem);
			new_y = nowItem->line().y1() + y;
			if (new_y < 0)
			{
				new_y = VIEW_HEIGHT + new_y;
				grid_h.pop_front();
				i--;
				range--;
				yGridMin--;
				yGridMax--;
				if (yGridMin == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(0, new_y, VIEW_WIDTH, new_y);
				this->addItem(nowItem);
				grid_h.push_back(nowItem);
			}
			else
			{
				nowItem->setLine(0, new_y, VIEW_WIDTH, new_y);
				this->addItem(nowItem);
			}
		}
	}
	else  //滑鼠向下拖曳(可視範圍向上增加)
	{
		range = 0;
		for (int i = 9; i >= range; i--)
		{
			nowItem = grid_h.at(i);
			this->removeItem(nowItem);
			new_y = nowItem->line().y1() + y;
			if (new_y > VIEW_HEIGHT)
			{
				new_y = new_y - VIEW_HEIGHT;
				grid_h.pop_back();
				i++;
				range++;
				yGridMin++;
				yGridMax++;
				if (yGridMax == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(0, new_y, VIEW_WIDTH, new_y);
				this->addItem(nowItem);
				grid_h.push_front(nowItem);
			}
			else
			{
				nowItem->setLine(0, new_y, VIEW_WIDTH, new_y);
				this->addItem(nowItem);
			}
		}
	}

	//水平移動
	if (x < 0)  //滑鼠向左拖曳(可視範圍向右增加)
	{
		range = 10;
		for (int i = 0; i < range; i++)
		{
			nowItem = grid_v.at(i);
			this->removeItem(nowItem);
			new_x = nowItem->line().x1() + x;
			if (new_x < 0)
			{
				new_x = VIEW_WIDTH + new_x;
				grid_v.pop_front();
				i--;
				range--;
				xGridMax++;
				xGridMin++;
				if (xGridMax == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(new_x, 0, new_x, VIEW_HEIGHT);
				this->addItem(nowItem);
				grid_v.push_back(nowItem);
			}
			else
			{
				nowItem->setLine(new_x, 0, new_x, VIEW_HEIGHT);
				this->addItem(nowItem);
			}
		}
	}
	else  //滑鼠向右拖曳(可視範圍向左增加)
	{
		range = 0;
		for (int i = 9; i >= range; i--)
		{
			nowItem = grid_v.at(i);
			this->removeItem(nowItem);
			new_x = nowItem->line().x1() + x;
			if (new_x > VIEW_WIDTH)
			{
				new_x = new_x - VIEW_WIDTH;
				grid_v.pop_back();
				i++;
				range++;
				xGridMin--;
				xGridMax--;
				if (xGridMin == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(new_x, 0, new_x, VIEW_HEIGHT);
				this->addItem(nowItem);
				grid_v.push_front(nowItem);
			}
			else
			{
				nowItem->setLine(new_x, 0, new_x, VIEW_HEIGHT);
				this->addItem(nowItem);
			}
		}
	}

	//移動數字//
	QGraphicsTextItem* nowTextItem;
	//垂直移動
	if (y < 0)  //滑鼠向上拖曳(可視範圍向下增加)
	{
		range = 10;
		for (int i = 0; i < range; i++)
		{
			nowTextItem = text_y.at(i);
			this->removeItem(nowTextItem);
			new_x = nowTextItem->pos().x() + x;
			new_y = nowTextItem->pos().y() + y;

			if (new_y < 0)
			{
				new_y = VIEW_HEIGHT + new_y;
				text_y.pop_front();
				i--;
				range--;

				nowTextItem->setPos(new_x, new_y);
				nowTextItem->setPlainText(QString::number(text_y.at(text_y.size() - 1)->toPlainText().toDouble() - scaleValue));
				this->addItem(nowTextItem);
				text_y.push_back(nowTextItem);
			}
			else
			{
				nowTextItem->setPos(new_x, new_y);
				this->addItem(nowTextItem);
			}
		}
	}
	else  //滑鼠向下拖曳(可視範圍向上增加)
	{
		range = 0;
		for (int i = 9; i >= range; i--)
		{
			nowTextItem = text_y.at(i);
			this->removeItem(nowTextItem);
			new_x = nowTextItem->pos().x() + x;
			new_y = nowTextItem->pos().y() + y;
			if (new_y > VIEW_HEIGHT)
			{
				new_y = new_y - VIEW_HEIGHT;
				text_y.pop_back();
				i++;
				range++;

				nowTextItem->setPos(new_x, new_y);
				nowTextItem->setPlainText(QString::number(text_y.at(0)->toPlainText().toDouble() + scaleValue));
				this->addItem(nowTextItem);
				text_y.push_front(nowTextItem);
			}
			else
			{
				nowTextItem->setPos(new_x, new_y);
				//nowTextItem->setLine(new_x, new_y, x2, new_y);
				this->addItem(nowTextItem);
			}
		}
	}

	//水平移動
	if (x < 0)  //滑鼠向左拖曳(可視範圍向右增加)
	{
		range = 10;
		for (int i = 0; i < range; i++)
		{
			nowTextItem = text_x.at(i);
			this->removeItem(nowTextItem);
			new_x = nowTextItem->pos().x() + x;
			new_y = nowTextItem->pos().y() + y;
			if (new_x < 0)
			{
				new_x = VIEW_WIDTH + new_x;
				text_x.pop_front();
				i--;
				range--;

				nowTextItem->setPos(new_x, new_y);
				nowTextItem->setPlainText(QString::number(text_x.at(text_x.size() - 1)->toPlainText().toDouble() + scaleValue));
				this->addItem(nowTextItem);
				text_x.push_back(nowTextItem);
			}
			else
			{
				nowTextItem->setPos(new_x, new_y);
				this->addItem(nowTextItem);
			}
		}
	}
	else  //滑鼠向右拖曳(可視範圍向左增加)
	{
		range = 0;
		for (int i = 9; i >= range; i--)
		{
			nowTextItem = text_x.at(i);
			this->removeItem(nowTextItem);
			new_x = nowTextItem->pos().x() + x;
			new_y = nowTextItem->pos().y() + y;
			if (new_x > VIEW_WIDTH)
			{
				new_x = new_x - VIEW_WIDTH;
				text_x.pop_back();
				i++;
				range++;

				nowTextItem->setPos(new_x, new_y);
				nowTextItem->setPlainText(QString::number(text_x.at(0)->toPlainText().toDouble() - scaleValue));
				this->addItem(nowTextItem);
				text_x.push_front(nowTextItem);
			}
			else
			{
				nowTextItem->setPos(new_x, new_y);
				this->addItem(nowTextItem);
			}
		}
	}

	//超出邊界時需吸付在邊邊//
	//y軸數字判斷
	if (xGridMax + xGridMin > 9)
	{
		new_x = 0;
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			this->removeItem(nowTextItem);
			nowTextItem->setPos(new_x, nowTextItem->pos().y());
			this->addItem(nowTextItem);
		}
	}
	else if (xGridMax + xGridMin < -9)
	{
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			new_x = VIEW_WIDTH - nowTextItem->boundingRect().width();
			this->removeItem(nowTextItem);
			nowTextItem->setPos(new_x, nowTextItem->pos().y());
			this->addItem(nowTextItem);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
			if (grid_v.at(i)->pen() == axisPen)
			{
				new_x = grid_v.at(i)->line().x1();
				break;
			}
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_y.at(i);
			this->removeItem(nowTextItem);
			nowTextItem->setPos(new_x, nowTextItem->pos().y());
			this->addItem(nowTextItem);
		}
	}

	//x軸數字判斷
	if (yGridMax + yGridMin > 9)
	{
		new_y = VIEW_HEIGHT - text_x.at(0)->boundingRect().height();
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			this->removeItem(nowTextItem);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);
			this->addItem(nowTextItem);
		}
	}
	else if (yGridMax + yGridMin < -9)
	{
		new_y = 0;
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			this->removeItem(nowTextItem);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);
			this->addItem(nowTextItem);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
			if (grid_h.at(i)->pen() == axisPen)
			{
				new_y = grid_h.at(i)->line().y1();
				break;
			}
		for (int i = 0; i < 10; i++)
		{
			nowTextItem = text_x.at(i);
			this->removeItem(nowTextItem);
			nowTextItem->setPos(nowTextItem->pos().x(), new_y);
			this->addItem(nowTextItem);
		}
	}

	manager.showGraph();
}

void GraphicsScene::zoomScene(QPointF point, double scale)
{
	if (scaleValue <= 0.0625 && scale > 1) return;

	const double zoom_x = point.x();
	const double zoom_y = point.y();
	scaleValue /= scale;
	x_min /= scale;
	x_max /= scale;
	y_min /= scale;
	y_max /= scale;
	for (int i = 0; i < 10; i++)
	{
		text_x.at(i)->setPlainText(QString::number(text_x.at(i)->toPlainText().toDouble() / scale));
		text_y.at(i)->setPlainText(QString::number(text_y.at(i)->toPlainText().toDouble() / scale));
	}
	if (scale > 1)
	{
		double dx = o_x - zoom_x;
		if (dx > 0)
			for (; dx > 500; dx -= 500)
				moveScene(500, 0);
		else
			for (; dx < -500; dx += 500)
				moveScene(-500, 0);
		moveScene(dx, 0);

		double dy = o_y - zoom_y;
		if (dy > 0)
			for (; dy > 500; dy -= 500)
				moveScene(0, 500);
		else
			for (; dy < -500; dy += 500)
				moveScene(0, -500);
		moveScene(0, dy);
	}
	else
	{
		double dx = (zoom_x - o_x) / 2;
		if (dx > 0)
			for (; dx > 500; dx -= 500)
				moveScene(500, 0);
		else
			for (; dx < -500; dx += 500)
				moveScene(-500, 0);
		moveScene(dx, 0);

		double dy = (zoom_y - o_y) / 2;
		if (dy > 0)
			for (; dy > 500; dy -= 500)
				moveScene(0, 500);
		else
			for (; dy < -500; dy += 500)
				moveScene(0, -500);
		moveScene(0, dy);
	}
}

vector<double> GraphicsScene::calculateGraph(double first, double last, int count, int index)
{
	vector<double> data;  //存函數數值
	double dx = (last - first) / (double)count;  //x的間距
	int diByZero_count = 0;  //除以零的次數
	double x = first;  //先將x設為第1項

	for (int i = 0; i <= count; i++)
	{
		try {
			data.push_back(manager.calculate(x, index));  //計算函數值
		}
		catch (divided_by_zero) {
			diByZero_count++;
			data.push_back(std::numeric_limits<double>::infinity());
			if (diByZero_count > count)  //如果函數的每一項都是除以0
				throw;
		}
		catch (std::exception& e) {
			string w = e.what();
			if (w != "cannot find variable")
			{
				if (w.at(0) == '-')
					data.push_back(-std::numeric_limits<double>::infinity());
				else
					data.push_back(std::numeric_limits<double>::infinity());
			}
		}
		x += dx;
	}

	return data;
}

double GraphicsScene::to_view_y(double value)
{
	if (value == INF || value == -INF)  //若數值為正負無限大
		return value;  //直接回傳
	return (y_max - value) * 521 / (y_max - y_min);  //回傳轉換後的數值
}

void GraphicsScene::draw()
{
	for (int i = 0; i < Storage::graphs.size(); i++)
	{
		try {
			//若找到y=的項，而且狀態為顯示
			if (Storage::graphs.at(i)->status == 1 && Storage::graphs.at(i)->name == "y")
			{
				vector<double> data;  //儲存函數值
				double precision;  //x的精確度(間距)

				if (scaleValue > 1)  //若縮放倍率大於1
					precision = PRECISION * scaleValue;  //精確度設為縮放倍率÷放大倍率
				else  //若縮放倍率不大於1
					precision = PRECISION;  //精確度設為預設倍率

				data = calculateGraph(x_min, x_max, precision, i);  //計算y的值，存入data

				double dx = (double)VIEW_WIDTH / precision;  //x在畫面的間距

				QPainterPath path;  //儲存函數路徑

				double x = 0;  //x初始設為0
				double y = to_view_y(data.at(0));  //y初始設為第0項
				bool need_move = false;

				double last_y;  //上一個y
				if (y != INF && y != -INF)  //若y不為正負無限大
				{
					path.moveTo(x, y);  //移動到(x,y)
					last_y = y;
				}
				else
					need_move = true;  //因為尚未移動，故將need_move設為true

				for (int i = 1; i <= precision; i++)
				{
					x += dx;
					y = to_view_y(data.at(i));
					
					if (need_move)
					{
						if (y != INF && y != -INF)
						{
							path.moveTo(x, y);
							need_move = false;
							last_y = y;
						}
					}
					else
					{
						if (y == INF || y == -INF || (fabs(y - last_y) > 5000))
						{
							need_move = true;
						}
						else
						{
							path.lineTo(x, y);
							last_y = y;
						}
					}
				}

				QGraphicsPathItem* pathItem;
				if (Storage::graphs.at(i)->graph != nullptr)
				{
					pathItem = Storage::graphs.at(i)->graph;
					this->removeItem(Storage::graphs.at(i)->graph);
				}
				else
					pathItem = new QGraphicsPathItem();
				pathItem->setPath(path);
				QPen pen;
				pen.setColor(Storage::graphs.at(i)->color);
				pen.setWidth(2);
				pathItem->setPen(pen);

				Storage::graphs.at(i)->graph = pathItem;
				this->addItem(pathItem);
			}
		}
		catch (divided_by_zero) {
			manager.removeGraph(i);
		}
	}
}

void GraphicsScene::removeGraph(int index)
{
	if (Storage::graphs.at(index)->graph != nullptr)
		this->removeItem(Storage::graphs.at(index)->graph);
	delete Storage::graphs.at(index)->graph;
	Storage::graphs.at(index)->graph = nullptr;
}

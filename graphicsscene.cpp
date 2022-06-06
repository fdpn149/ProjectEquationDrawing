#include "graphicsscene.h"
#include "manager.h"

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
		if (i == 5) continue;  //跳過坐標軸的線
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

	//draw();  //畫正弦波
}

GraphicsScene::~GraphicsScene()
{

}

vector<double> GraphicsScene::create_data(double start, double end, int segment_count, int index)
{
	double delta = (end - start) / (double)segment_count;
	size_t point_count = segment_count + 1;

	vector<double> data;
	double x = start;

	for (size_t i = 0; i < point_count; ++i)
	{
		try {
			data.push_back(manager.calculate(x, index));
		}
		catch(std::exception& e){
			string w = e.what();
			if (w.at(0) == '-')
				data.push_back(std::numeric_limits<double>::min());
			else
				data.push_back(std::numeric_limits<double>::max());
		}
		x += delta;
	}

	return data;
}

void GraphicsScene::moveScene(int x, int y)
{
	QGraphicsLineItem* nowItem;
	double x1, y1, x2, y2;
	QPen axisPen;
	QPen gridPen;
	axisPen.setWidth(3);
	int range;
	x_min -= x * 10.0 / (double)VIEW_WIDTH;
	x_max -= x * 10.0 / (double)VIEW_WIDTH;
	y_min += y * 10.0 / (double)VIEW_HEIGHT;
	y_max += y * 10.0 / (double)VIEW_HEIGHT;
	//垂直移動
	if (y < 0)  //滑鼠向上拖曳(可視範圍向下增加)
	{
		range = 10;
		for (int i = 0; i < range; i++)
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
				range--;
				yGridMin--;
				yGridMax--;
				if (yGridMin == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
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
	}
	else  //滑鼠向下拖曳(可視範圍向上增加)
	{
		range = 0;
		for (int i = 9; i >= range; i--)
		{
			nowItem = grid_h.at(i);
			this->removeItem(nowItem);
			x1 = nowItem->line().x1();
			y1 = nowItem->line().y1() + y;
			x2 = nowItem->line().x2();
			if (y1 > VIEW_HEIGHT)
			{
				y1 = y1 - VIEW_HEIGHT;
				grid_h.pop_back();
				i++;
				range++;
				yGridMin++;
				yGridMax++;
				if (yGridMax == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(x1, y1, x2, y1);
				this->addItem(nowItem);
				grid_h.push_front(nowItem);
			}
			else
			{
				nowItem->setLine(x1, y1, x2, y1);
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
			y1 = nowItem->line().y1();
			x1 = nowItem->line().x1() + x;
			y2 = nowItem->line().y2();
			if (x1 < 0)
			{
				x1 = VIEW_WIDTH + x1;
				grid_v.pop_front();
				i--;
				range--;
				xGridMax++;
				xGridMin++;
				if (xGridMax == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(x1, y1, x1, y2);
				this->addItem(nowItem);
				grid_v.push_back(nowItem);
			}
			else
			{
				nowItem->setLine(x1, y1, x1, y2);
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
			y1 = nowItem->line().y1();
			x1 = nowItem->line().x1() + x;
			y2 = nowItem->line().y2();
			if (x1 > VIEW_WIDTH)
			{
				x1 = x1 - VIEW_WIDTH;
				grid_v.pop_back();
				i++;
				range++;
				xGridMin--;
				xGridMax--;
				if (xGridMin == 0)
					nowItem->setPen(axisPen);
				else
					nowItem->setPen(gridPen);
				nowItem->setLine(x1, y1, x1, y2);
				this->addItem(nowItem);
				grid_v.push_front(nowItem);
			}
			else
			{
				nowItem->setLine(x1, y1, x1, y2);
				this->addItem(nowItem);
			}
		}
	}
	manager.showGraph();
}

void GraphicsScene::draw()
{
	for (int i = 0; i < Storage::variable.size(); i++)
	{
		if (Storage::variable.at(i).first == "y")
		{
			vector<double> data = create_data(x_min, x_max, 100.0, i);

			double delta_x = (double)VIEW_WIDTH / 100.0;
			double delta_y = -(double)VIEW_HEIGHT / 10.0;

			double start_x = 0.0;

			QPainterPath path;
			path.moveTo(start_x, data[0] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0);
			bool skip = false;
			for (size_t i = 1; i < 101; ++i)
			{
				start_x += delta_x;
				if (skip)
				{
					path.moveTo(start_x, data[i] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0);
					skip = false;
				}

				if (data[i] == std::numeric_limits<double>::min() || data[i] == std::numeric_limits<double>::max())
					skip = true;
				else
					path.lineTo(start_x, data[i] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0);
			}
			QGraphicsPathItem* pathItem = new QGraphicsPathItem();
			pathItem->setPath(path);
			if (Storage::outputGraph.find(i) != Storage::outputGraph.end())
				this->removeItem(Storage::outputGraph[i]);
			this->addItem(pathItem);
			Storage::outputGraph[i] = pathItem;
		}
	}
}

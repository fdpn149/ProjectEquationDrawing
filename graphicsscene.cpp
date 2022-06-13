#include "graphicsscene.h"
#include "manager.h"

GraphicsScene::GraphicsScene(QObject* parent, int width, int height)
	: QGraphicsScene(parent), VIEW_WIDTH(width), VIEW_HEIGHT(height), CENTER_X(width / 2.0), CENTER_Y(height / 2.0)
{
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

vector<double> GraphicsScene::create_data(double start, double end, int segment_count, int index)
{
	double delta = (end - start) / (double)segment_count;

	vector<double> data;
	double x = start;

	int diByZero_count = 0;
	for (size_t i = 0; i <= segment_count; ++i)
	{
		try {
			data.push_back(manager.calculate(x, index));
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
		catch (divided_by_zero) {
			diByZero_count++;
			data.push_back(std::numeric_limits<double>::infinity());
			if (diByZero_count > segment_count)
				throw;
		}
		x += delta;
	}

	return data;
}

void GraphicsScene::moveScene(int x, int y)
{
	double new_x, new_y;
	QPen axisPen;
	QPen gridPen;
	axisPen.setWidth(3);
	int range;

	//調整xy的顯示範圍
	x_min -= x * 10.0 / (double)VIEW_WIDTH;
	x_max -= x * 10.0 / (double)VIEW_WIDTH;
	y_min += y * 10.0 / (double)VIEW_HEIGHT;
	y_max += y * 10.0 / (double)VIEW_HEIGHT;

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
				nowTextItem->setPlainText(QString::number(text_y.at(text_y.size() - 1)->toPlainText().toDouble() - 1));
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
				nowTextItem->setPlainText(QString::number(text_y.at(0)->toPlainText().toDouble() + 1));
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
				nowTextItem->setPlainText(QString::number(text_x.at(text_x.size() - 1)->toPlainText().toDouble() + 1));
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
				nowTextItem->setPlainText(QString::number(text_x.at(0)->toPlainText().toDouble() - 1));
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

void GraphicsScene::zoomScene(QPointF point)
{
	const double zoom_x = point.x();
	const double zoom_y = point.y();
	moveScene(CENTER_X - zoom_x, CENTER_Y - zoom_y);
}

void GraphicsScene::draw()
{
	double inf = std::numeric_limits<double>::infinity();

	for (int i = 0; i < Storage::graphs.size(); i++)
	{
		try {
			if (Storage::graphs.at(i)->status == 1 && Storage::graphs.at(i)->name == "y")
			{
				vector<double> data = create_data(x_min, x_max, 500.0, i);

				double delta_x = (double)VIEW_WIDTH / 500.0;
				double delta_y = -(double)VIEW_HEIGHT / 10.0;

				double start_x = 0.0;

				QPainterPath path;

				double num = data[0] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0;
				if (num != inf && num != -inf)
					path.moveTo(start_x, num);

				bool skip = false;
				for (size_t i = 1; i <= 500; ++i)
				{
					start_x += delta_x;
					if (skip)
					{
						num = data[i] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0;
						if (num != inf && num != -inf)
						{
							path.moveTo(start_x, num);
							skip = false;
						}
					}

					if (data[i] == inf || data[i] == -inf)
						skip = true;
					else
						path.lineTo(start_x, data[i] * delta_y + CENTER_Y + (y_max - 5) * VIEW_WIDTH / 10.0);
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

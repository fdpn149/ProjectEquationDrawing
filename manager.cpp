#include "manager.h"
#include "viewer.h"
#include "graphicsscene.h"
#include<ctime>
#include<cstdlib>

Manager manager;

Manager::Manager()
{
	std::srand(unsigned(time(0)));
}

Manager::~Manager()
{
	delete viewer;
}

void Manager::clearQueue(queue<string>& q)
{
	queue<string> empty;
	swap(empty, q);
}

void Manager::input(string input, QListWidgetItem* item, int nowRow)
{
	clearQueue(storage.infix);  //清除中序Queue
	storage.postfix.clear();  //清除後序Vector
	string origin_name = Storage::graphs.at(nowRow)->name;
	Storage::graphs.at(nowRow)->clear();
	string name = parser.parseInput(input, storage, nowRow);  //輸入解析

	if (name != "")
	{
		viewer->changeItemIcon(item, 0, Storage::graphs.at(nowRow)->color);

		parser.toPostfix(storage.infix, storage.postfix);  //中序轉後序


		
		Storage::graphs.at(nowRow)->name = name;  //設定名字
		Storage::graphs.at(nowRow)->postfix = storage.postfix;  //儲存後序式

		if (origin_name != name)
		{
			//檢查是否有變數重複定義
			for (int i = 0; i < Storage::graphs.size(); i++)
			{
				string now_name = Storage::graphs.at(i)->name;
				if (now_name != "y")  //跳過檢查y
				{
					int total = std::count_if(Storage::graphs.begin(), Storage::graphs.end(),
						[now_name](Graph* g) {return now_name == g->name; });
					int k = Storage::graphs.size() - 1;
					for (int j = total; j > 1; j--)
					{
						while (Storage::graphs.at(k)->name != now_name) k--;
						Storage::graphs.at(k)->clear();
						viewer->changeItemIcon(k, -1, Storage::graphs.at(k)->color);
					}
				}

				if (!Storage::graphs.at(i)->postfix.empty())
				{
					for (int j = 0; j < Storage::graphs.at(i)->postfix.size(); j++)
					{
						if (Storage::graphs.at(i)->postfix.at(j) == origin_name)
						{
							Storage::graphs.at(i)->clear();
							viewer->changeItemIcon(i, -1, Storage::graphs.at(i)->color);
						}
					}
				}
			}
		}
	}
	else
	{
		viewer->changeItemIcon(item, -1, Storage::graphs.at(nowRow)->color);
		clearQueue(storage.infix);
	}
}

double Manager::calculate(double x, int index)
{
	int rindex = Storage::graphs.size() - index - 1;
	try {
		return parser.calculate(x, Storage::graphs.rbegin() + rindex, Storage::graphs.rend());
	}
	catch (std::exception& e) {
		viewer->addText(e.what());
		throw;
	}
}

void Manager::showGraph()
{
	viewer->showGraph();
}

void Manager::start()
{
	viewer = new Viewer();
}


void Manager::addNewItem()
{
	int r = rand() % 256;
	int g = rand() % 256;
	int b = rand() % 256;

	//限制顏色深淺(綠色*3+紅色*2+藍色*1)
	while (g * 3 + r * 2 + b > 1086 || g * 3 + r * 2 + b < 192)
	{
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	}
	QColor col = QColor(r, g, b);
	viewer->addItem(col);
	Graph* newGraph = new Graph(col);
	Storage::graphs.push_back(newGraph);
}

void Manager::editItem(QListWidgetItem* item, int nowRow)
{
	//Storage::graphs.at(nowRow)->clear();
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	viewer->editItem(item, Storage::graphs.at(nowRow)->color);
}

void Manager::removeItem(QListWidgetItem* item, int nowRow)
{
	viewer->removeGraph(nowRow);
	Storage::graphs.erase(Storage::graphs.begin() + nowRow);
}


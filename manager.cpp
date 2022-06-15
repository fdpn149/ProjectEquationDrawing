#include "manager.h"
#include "viewer.h"
#include "graphicsscene.h"
#include<ctime>
#include<cstdlib>
#include<set>
using std::set;

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
	for (int i = 0; i < Storage::graphs.size(); i++)
		viewer->removeGraph(i);
	Storage::graphs.at(nowRow)->clear();
	string name = parser.parseInput(input, storage, nowRow);  //輸入解析

	if (name != "")
	{
		Storage::graphs.at(nowRow)->status = 1;
		viewer->changeItemIcon(item, 0, Storage::graphs.at(nowRow)->color);
		parser.toPostfix(storage.infix, storage.postfix);  //中序轉後序
		Storage::graphs.at(nowRow)->name = name;  //設定名字
		Storage::graphs.at(nowRow)->postfix = storage.postfix;  //儲存後序式
	}
	else
	{
		viewer->changeItemIcon(item, -1, Storage::graphs.at(nowRow)->color);
		Storage::graphs.at(nowRow)->status = -1;
		if (origin_name == "y" || origin_name == "x")
			viewer->removeGraph(nowRow);
		clearQueue(storage.infix);
	}
}

double Manager::calculate(double num, char type, int index)
{
	int rindex = Storage::graphs.size() - index - 1;
	return parser.calculate(num, type, Storage::graphs.rbegin() + rindex, Storage::graphs.rend());
}

void Manager::showGraph()
{
	viewer->showGraph();
}

void Manager::checkError(int nowRow)
{
	set<string> var_before;  //這一列之前/之後的變數集合
	for (int i = 0; i <= nowRow; i++)
	{
		if (Storage::graphs.at(i)->name != "y" && Storage::graphs.at(i)->name != "x" && Storage::graphs.at(i)->status == 1)
			var_before.insert(Storage::graphs.at(i)->name);
	}

	for (int i = nowRow + 1; i < Storage::graphs.size(); i++)
	{
		bool error = false;
		if (Storage::graphs.at(i)->name == "" || var_before.find(Storage::graphs.at(i)->name) != var_before.end())  //重複定義變數
		{
			Storage::graphs.at(i)->clear();
			error = true;
		}
		else
		{
			for (string s : Storage::graphs.at(i)->postfix)
			{
				if (isalpha(s.at(0)) && s != "sin" && s != "cos" && s != "tan" && s != "x" && s != "y")
				{
					if (var_before.find(s) == var_before.end())
					{
						error = true;
						break;
					}
				}
			}
		}
		if (error)
		{
			viewer->changeItemIcon(i, -1, Storage::graphs.at(i)->color);
			Storage::graphs.at(i)->status = -1;
			if (Storage::graphs.at(i)->name == "y" || Storage::graphs.at(i)->name == "x")
			{
				viewer->removeGraph(i);
				Storage::graphs.at(i)->graph = nullptr;
			}
		}
		else
		{
			viewer->changeItemIcon(i, 0, Storage::graphs.at(i)->color);
			Storage::graphs.at(i)->status = 1;
			if (Storage::graphs.at(i)->name != "y" && Storage::graphs.at(i)->name != "x")
				var_before.insert(Storage::graphs.at(i)->name);
		}
	}
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
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	viewer->editItem(item, Storage::graphs.at(nowRow)->color);
}

void Manager::removeItem(int nowRow)
{
	string name = Storage::graphs.at(nowRow)->name;
	if (name == "y" || name == "x")
		viewer->removeGraph(nowRow);
	Storage::graphs.erase(Storage::graphs.begin() + nowRow);
}

void Manager::removeGraph(int index)
{
	viewer->changeItemIcon(index, -1, Storage::graphs.at(index)->color);
	Storage::graphs.at(index)->status = -1;
	viewer->removeGraph(index);
}


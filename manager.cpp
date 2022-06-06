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
	clearQueue(storage.infix);
	storage.postfix.clear();
	string name;
	name = parser.parseInput(input, storage, nowRow);


	if (name != "")
	{
		viewer->changeItemIcon(item, 0, storage.color.at(nowRow));

		/*queue<string> display(storage.infix);
		viewer->changeText(name + " =");
		while (display.size() > 0)
		{
			viewer->addText(display.front());
			display.pop();
		}*/

		parser.toPostfix(storage.infix, storage.postfix);

		/*viewer->addText("\n");
		for (auto it = storage.postfix.begin(); it != storage.postfix.end(); it++)
			viewer->addText(*it);*/

		storage.variable.at(nowRow) = { name, storage.postfix };

		//檢查是否有變數重複定義
		for (int i = 0; i < Storage::variable.size(); i++)
		{
			string now_name = Storage::variable.at(i).first;
			if (now_name != "y")
			{
				int total = std::count_if(Storage::variable.begin(), Storage::variable.end(),
					[now_name](pair<string, vector<string>> p) {return now_name == p.first; });
				int k = Storage::variable.size() - 1;
				for (int j = total; j > 1; j--)
				{
					while (Storage::variable.at(k).first != now_name) k--;
					Storage::variable.at(k).first = "";
					Storage::variable.at(k).second = vector<string>();
					viewer->changeItemIcon(k, -1, storage.color.at(k));
				}
			}
		}
	}
	else
	{
		viewer->changeItemIcon(item, -1, storage.color.at(nowRow));
		//viewer->changeText("ERROR");
		clearQueue(storage.infix);
	}
}

double Manager::calculate(double x, int index)
{
	int rindex = Storage::variable.size() - index - 1;
	try {
		return parser.calculate(x, Storage::variable.rbegin() + rindex, Storage::variable.rend());
		//viewer->addText("\n");
		//viewer->addText(result);
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
	storage.color.push_back(col);
	storage.variable.push_back({ "",vector<string>() });
}

void Manager::editItem(QListWidgetItem* item, int nowRow)
{
	string name = parser.getVarName(item->text().toStdString());
	storage.variable.at(nowRow) = { "", vector<string>() };
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	viewer->editItem(item, storage.color.at(nowRow));
}

void Manager::removeItem(QListWidgetItem* item, int nowRow)
{
	storage.color.erase(storage.color.begin() + nowRow);
	string name = parser.getVarName(item->text().toStdString());
	storage.variable.erase(storage.variable.begin() + nowRow);
}


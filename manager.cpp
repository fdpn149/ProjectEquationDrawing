#include "manager.h"
#include "viewer.h"
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
	name = parser.parseInput(input, storage);

	if (name != "")
	{
		viewer->changeItemIcon(item, 0, storage.color.at(nowRow));
		queue<string> display(storage.infix);
		viewer->changeText(name + " =");

		while(display.size() > 0)
		{
			viewer->addText(display.front());
			display.pop();
		}

		viewer->addText("\n");
		parser.toPostfix(storage.infix, storage.postfix);
		for (auto it = storage.postfix.begin(); it != storage.postfix.end(); it++)
			viewer->addText(*it);

		if (name == "y")
			storage.outputGraphs[nowRow] = storage.postfix;
		else
			storage.variable[name] = storage.postfix;
	}
	else
	{
		viewer->changeItemIcon(item, -1, storage.color.at(nowRow));
		viewer->changeText("ERROR");
		clearQueue(storage.infix);
	}
}

void Manager::calculate(double x, string name)
{
	viewer->addText("\n");
	try {
		if (storage.variable.find(name) != storage.variable.end())
		{
			string result = std::to_string(parser.calculate(x, storage.variable[name]));
			viewer->addText(result);
		}
	}
	catch (std::exception& e) {
		viewer->addText(e.what());
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
	storage.color.push_back(col);
}

void Manager::editItem(QListWidgetItem* item, int nowRow)
{
	string name = parser.getVarName(item->text().toStdString());
	if (name == "y")
		storage.outputGraphs.erase(nowRow);
	else if(name != "")
		Storage::variable.erase(name);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	viewer->editItem(item, storage.color.at(nowRow));
}

void Manager::removeItem(QListWidgetItem* item, int nowRow)
{
	storage.color.erase(storage.color.begin() + nowRow);
	string name = parser.getVarName(item->text().toStdString());
	if(name == "y")
		storage.outputGraphs.erase(nowRow);
	else if (name != "")
		Storage::variable.erase(name);
}


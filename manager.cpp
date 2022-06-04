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

void Manager::input(string input)
{
	clearQueue(storage.infix);
	storage.postfix.clear();

	result = parser.parseInput(input, storage);
	if (result != -1)
	{
		queue<string> display(storage.infix);
		viewer->changeText(display.front());
		display.pop();
		while(display.size() > 0)
		{
			viewer->addText(display.front());
			display.pop();
		}

		parser.toPostfix(storage.infix, storage.postfix);
		system("cls");
		for (auto it = storage.postfix.begin(); it != storage.postfix.end(); it++)
			std::cout << *it << " ";
	}
	else
	{
		viewer->changeText("ERROR");
		clearQueue(storage.infix);
	}
}

void Manager::start()
{
	viewer = new Viewer();
}

void Manager::changeIcon(QListWidgetItem* item, int nowRow)
{
	viewer->changeItemIcon(item, result, color.at(nowRow));
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
	color.push_back(col);
}

void Manager::editItem(QListWidgetItem* item, int nowRow)
{
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	viewer->editItem(item, color.at(nowRow));
}

void Manager::removeItem(int nowRow)
{
	color.erase(color.begin() + nowRow);
}


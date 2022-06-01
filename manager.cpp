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

void Manager::parseInput(string input)
{
	Parser::part_input.clear();
	result = parser.parseInput(input, storage);
	if (result != -1)
	{
		viewer->changeText(Parser::part_input.at(0));
		for (int i = 1; i < Parser::part_input.size(); i++)
		{
			viewer->addText(Parser::part_input.at(i));
		}
	}
	else
	{
		viewer->changeText("ERROR");
		Parser::part_input.clear();
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

	while (g * 3 + r * 2 + b > 894)
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


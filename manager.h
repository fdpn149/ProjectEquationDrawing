#ifndef MANAGER_H
#define MANAGER_H
#include <iostream>
#include "storage.h"
#include "parser.h"
#include "viewer.h"
using std::string;

class Manager
{
	Parser parser;
	Storage storage;
	Viewer* viewer;
	vector<QColor> color;
	int result = 0;
public:
	Manager();
	~Manager();
	void start();
	void parseInput(string input);
	void addNewItem();
	void editItem(QListWidgetItem* item, int nowRow);
	void removeItem(int nowRow);
	void changeIcon(QListWidgetItem* item, int nowRow);
};

extern Manager manager;

#endif // MANAGER_H

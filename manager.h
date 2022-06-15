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
	void clearQueue(queue<string>& q);
public:
	Manager();
	~Manager();
	void start();
	void input(string input, QListWidgetItem* item, int nowRow);
	void addNewItem();
	void editItem(QListWidgetItem* item, int nowRow);
	void removeItem(int nowRow);
	void removeGraph(int index);
	double calculate(double num, char type, int index);
	void showGraph();
	void checkError(int nowRow);
};

extern Manager manager;

#endif // MANAGER_H

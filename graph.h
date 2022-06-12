#pragma once
#include <iostream>
#include <QColor>
#include <vector>
#include <QGraphicsPathItem>
using std::string;
using std::vector;

class Graph
{
public:
	string name;
	vector<string> postfix;
	QColor color;
	QGraphicsPathItem* graph;
	int status;  //-1:Error, 0:empty, 1:correct

	Graph(QColor c) : color(c), graph(nullptr), status(1) {}
	void clear();
};


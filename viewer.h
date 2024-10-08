#ifndef VIEWER_H
#define VIEWER_H
#include "mainwindow.h"
#include <iostream>
using std::string;

class Viewer
{
	MainWindow window;
public:
	Viewer();
	void addItem(QColor color);
	void editItem(QListWidgetItem* item, QColor color);
	void changeItemIcon(QListWidgetItem* item, int type, QColor color);
	void changeItemIcon(int index, int type, QColor color);
	void showGraph();
	void removeGraph(int index);
};

#endif // VIEWER_H

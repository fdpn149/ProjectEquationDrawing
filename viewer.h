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
	void changeText(string str);
	void addText(string str);
	void addItem(QColor color);
	void editItem(QListWidgetItem* item, QColor color);
	void changeItemIcon(QListWidgetItem* item, int type, QColor color);
};

#endif // VIEWER_H

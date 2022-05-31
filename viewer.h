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
	void changeButtonText(string str);
};

#endif // VIEWER_H

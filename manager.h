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
public:
	Manager();
	~Manager();
	void start();
	void parseInput(string input);
};

extern Manager manager;

#endif // MANAGER_H

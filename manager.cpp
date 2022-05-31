#include "manager.h"
#include "viewer.h"

Manager manager;

Manager::Manager()
{
	
}

Manager::~Manager()
{
	delete viewer;
}

void Manager::parseInput(string input)
{
	
	viewer->changeButtonText(std::to_string(parser.parseInput(input, storage)));
}

void Manager::start()
{
	viewer = new Viewer();
}


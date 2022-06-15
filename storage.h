#ifndef STORAGE_H
#define STORAGE_H
#include <vector>
#include <queue>
#include <string>
#include "graph.h"
using std::vector;
using std::queue;
using std::string;
class Storage
{
public:
	Storage() {}
	~Storage();
	static vector<Graph*> graphs;
	queue<string> infix;  //儲存中序式
	vector<string> postfix;  //儲存後序式
};

#endif // STORAGE_H

#include "storage.h"

vector<Graph*> Storage::graphs;

Storage::~Storage()
{
	for (Graph* g : graphs)
		delete g;
}

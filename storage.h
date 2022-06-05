#ifndef STORAGE_H
#define STORAGE_H
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <QColor>
using std::map;
using std::vector;
using std::queue;
using std::string;
using std::pair;
class Storage
{
public:
	Storage() {}
	static vector<pair<string, vector<string>>> variable;
	queue<string> infix;  //儲存中序式
	vector<string> postfix;  //儲存後序式
	vector<QColor> color;
};

#endif // STORAGE_H

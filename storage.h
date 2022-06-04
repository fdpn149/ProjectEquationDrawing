#ifndef STORAGE_H
#define STORAGE_H
#include <map>
#include <vector>
#include <queue>
#include <string>
using std::map;
using std::vector;
using std::queue;
using std::string;

class Storage
{
public:
    Storage(){}
    map<string, double> variable;  //儲存變數
    vector<string> outputGraphs;  //儲存輸出函式
    queue<string> infix;  //儲存中序式
    vector<string> postfix;  //儲存後序式
};

#endif // STORAGE_H

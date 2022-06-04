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

class Storage
{
public:
    Storage(){}
    static map<string, vector<string>> variable;  //儲存變數<變數名稱,後序式>
    map<int, vector<string>> outputGraphs;  //儲存輸出函式
    queue<string> infix;  //儲存中序式
    vector<string> postfix;  //儲存後序式
    vector<QColor> color;
};

#endif // STORAGE_H

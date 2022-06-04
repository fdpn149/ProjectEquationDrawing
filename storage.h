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
    static map<string, vector<string>> variable;  //�x�s�ܼ�<�ܼƦW��,��Ǧ�>
    map<int, vector<string>> outputGraphs;  //�x�s��X�禡
    queue<string> infix;  //�x�s���Ǧ�
    vector<string> postfix;  //�x�s��Ǧ�
    vector<QColor> color;
};

#endif // STORAGE_H

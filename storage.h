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
    map<string, double> variable;  //�x�s�ܼ�
    vector<string> outputGraphs;  //�x�s��X�禡
    queue<string> infix;  //�x�s���Ǧ�
    vector<string> postfix;  //�x�s��Ǧ�
};

#endif // STORAGE_H

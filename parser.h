#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include "storage.h"
using std::vector;
using std::queue;
using std::map;
using std::string;

class Parser
{
    bool isNumber(string str);
public:
    Parser();
    int parseInput(string input, Storage& storage);
    int getWeight(string symbol);  //得到符號的優先值
    void toPostfix(queue<string> infix, vector<string>& postfix);
};

#endif // PARSER_H

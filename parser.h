#ifndef PARSER_H
#define PARSER_H
#include "storage.h"
#include <iostream>
#include <map>
using std::map;
using std::string;

class Parser
{
public:
    Parser();
    int parseInput(string input, Storage& storage);
};

#endif // PARSER_H

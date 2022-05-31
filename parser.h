#ifndef PARSER_H
#define PARSER_H
#include "storage.h"
#include <iostream>
using std::string;

class Parser
{
public:
    Parser(){}
    void parseInput(string input, Storage& storage);
};

#endif // PARSER_H

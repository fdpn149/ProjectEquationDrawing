#ifndef PARSER_H
#define PARSER_H
#include "storage.h"
#include <iostream>
#include <vector>
using std::vector;
using std::string;

class Parser
{
public:
    Parser();
    int parseInput(string input, Storage& storage);
    static vector<string> part_input;
};

#endif // PARSER_H

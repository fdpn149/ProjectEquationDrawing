#ifndef MANAGER_H
#define MANAGER_H
#include <iostream>
#include "storage.h"
#include "parser.h"
#include "viewer.h"
using std::string;

class Manager
{
    Parser parser;
    Storage storage;
public:
    Manager(){}
    void parseInput(string input) { parser.parseInput(input, storage); }
};

#endif // MANAGER_H

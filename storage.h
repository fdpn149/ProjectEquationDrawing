#ifndef STORAGE_H
#define STORAGE_H
#include <map>
#include <vector>
#include <string>
using std::map;
using std::vector;
using std::string;

class Storage
{
    map<string, double> variable;
    vector<double> outputGraphs;
public:
    Storage(){}

};

#endif // STORAGE_H

#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <exception>
#include "storage.h"

using std::string;
using std::pair;

class Parser
{
	bool isVarNum(string str);  //判斷是否是變數或數值
	bool isOperator(string str);  //判斷是否是運算符號
public:
	Parser();
	string getVarName(string input);  //取得等號前的變數名稱
	string parseInput(string input, Storage& storage, int nowRow);  //輸入處理
	int getWeight(string symbol);  //得到符號的優先值
	void toPostfix(queue<string> infix, vector<string>& postfix);  //轉成後序式
	double calculate(double x, vector<Graph*>::reverse_iterator rbegin, vector<Graph*>::reverse_iterator rend);  //計算數值

	template<class T>
	T findVariable(T begin, T end, string name);  //尋找變數的位置
};

class calculate_error : public std::exception
{
	string s;
	virtual const char* what() const throw()
	{
		return s.c_str();
	}
public:
	calculate_error(string str) : s(str) {}
};

class divided_by_zero{};

#endif // PARSER_H

template<class T>
T Parser::findVariable(T begin, T end, string name)
{
	T it = begin;
	while (it != end)
	{
		if ((*it)->name == name)
			return it;
		it++;
	}
	return it;
}

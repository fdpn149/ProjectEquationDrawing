#include "parser.h"
#include <stack>
using std::stack;

bool Parser::isNumber(string str)
{
	if (str == "sin" || str == "cos")
		return false;
	if (isalpha(str.at(0)))  //如果是變數
		return true;
	bool dot = false;
	for (int i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.at(i)))  //遇到非數字
		{
			if (!dot && str.at(i) == '.')  //若第一次遇到.
				dot = true;
			else if (dot)
				return false;
			else
				return false;
		}
	}
	return true;
}

Parser::Parser()
{

}

int Parser::parseInput(string input, Storage& storage)
{
	if (!std::isalpha(input.at(0))) return -1;  //若輸入的第1字不是英文，回傳-1

	string v1 = "";  //開頭的變數名稱
	int eq_pos;  //等號的位置

	//找等號
	for (int i = 1; i < input.size() - 1; i++)
	{
		if (!isalnum(input.at(i)))
		{
			if (input.at(i) == '=')  //若遇到=
			{
				v1 = input.substr(0, i);
				eq_pos = i;
				break;
			}
			else
				return -1;
		}
	}
	if (v1.empty() || v1 == "sin" || v1 == "cos") return -1;  //若v1為空字串，回傳-1
	if (storage.variable.find(v1) != storage.variable.end()) return -1;  //變數重複定義

	int par_count = 0;  //計算上下括號數
	int next_code = 123;  //下一個有效字元的代碼 #初始為(-0axs
	int now_code = 0;

	for (int i = eq_pos + 1; i < input.size(); i++)
	{
		//若是左括號
		if ((next_code & 1) >= 1 && input.at(i) == '(')
		{
			par_count++;
			now_code = 1;
			next_code = 123;
			storage.infix.push("(");
			continue;
		}
		//若是負號或減號
		if ((next_code & 2) >= 1 && input.at(i) == '-')
		{
			if (now_code == 0 || now_code == 1)  //是=或(
				storage.infix.push("--");  //負號
			else
				storage.infix.push("-");  //減號
			now_code = 2;
			next_code = 121;
			continue;
		}
		//加乘除冪
		if ((next_code & 4) >= 1 && (input.at(i) == '+' || input.at(i) == '*'
			|| input.at(i) == '/' || input.at(i) == '^'))
		{
			now_code = 4;
			next_code = 121;
			storage.infix.push(string(1, input.at(i)));
			continue;
		}
		//數字
		if ((next_code & 8) >= 1 && std::isdigit(input.at(i)))
		{
			int from = i;

			bool flag = false;
			//將i移到不是數字的地方
			while (i < input.length() && (std::isdigit(input.at(i)) || input.at(i) == '.'))
			{
				if (!flag && input.at(i) == '.')
					flag = true;
				else if (flag && input.at(i) == '.')
					return -1;
				i++;
			}
			if (i-1 >= 0 && input.at(i-1) == '.')
				return -1;

			now_code = 8;
			next_code = 398;

			string str = input.substr(from, i - from);  //截取
			i--;

			storage.infix.push(str);
			continue;
		}
		//英文(sin/cos=16, x=32, 其它=64)
		if ((next_code & 112) >= 1 && std::isalpha(input.at(i)))
		{
			//sin/cos判斷
			if (input.substr(i, 3) == "sin" || input.substr(i, 3) == "cos")
			{
				if ((next_code & 16) >= 1 && i + 3 < input.length() && input.at(i + 3) == '(')  //若sin/cos的下個字為(
				{
					storage.infix.push(input.substr(i, 3));
					i += 2;
					now_code = 16;
					next_code = 1;
					continue;
				}
				else if (i + 3 < input.length() && !std::isalpha(input.at(i + 3)) && !std::isdigit(input.at(i + 3)))  //若下個字不為英文或數字(不是變數)
				{
					return -1;
				}
			}
			//x判斷
			else if (input.at(i) == 'x')
			{
				//若為單純的x
				if ((next_code & 32) >= 1 && (i + 1 >= input.length() || !std::isalnum(input.at(i + 1))))
				{
					now_code = 32;
					next_code = 262;
					storage.infix.push("x");
					continue;
				}
			}

			//其它變數判斷//
			int from = i;

			//將i移到不是變數的地方
			while (i < input.length() && std::isalnum(input.at(i)))
				i++;
			string name = input.substr(from, i - from);  //截取變數名稱
			i--;

			//TODO:尋找變數是否存在

			storage.infix.push(name);
			now_code = 64;
			next_code = 262;
			continue;
		}
		//小數點
		if ((next_code & 128) >= 1 && input.at(i) == '.')
		{
			now_code = 128;
			next_code = 8;
			storage.infix.push(".");
			continue;
		}
		//右括號
		if ((next_code & 256) >= 1 && input.at(i) == ')')
		{
			par_count--;

			if (par_count < 0) return -1;  //判斷是否多括

			now_code = 256;
			next_code = 262;
			storage.infix.push(")");
			continue;
		}

		return -1;
	}

	if (par_count > 0)
		return -1;



	if ((now_code & 488) == 0)
		return -1;


	return 0;
}

int Parser::getWeight(string symbol)
{
	if (symbol == "sin" || symbol == "cos")
		return 4;
	if (symbol == "^")
		return 3;
	if (symbol == "*" || symbol == "/")
		return 2;
	if (symbol == "+" || symbol.at(0) == '-')
		return 1;
	if (symbol == "(")
		return 0;
	return -1;
}

void Parser::toPostfix(queue<string> infix, vector<string>& postfix)
{
	stack<string> symbolStack;  //符號的堆疊

	//將infix的每一項都做一次，做完就拿掉
	while (!infix.empty())
	{
		if (isNumber(infix.front()))
			postfix.push_back(infix.front());
		else if (symbolStack.empty())
			symbolStack.push(infix.front());
		else if (infix.front() == "(")
		{
			symbolStack.push("(");
		}
		else if (infix.front() == ")")
		{
			while (symbolStack.top() != "(")
			{
				postfix.push_back(symbolStack.top());
				symbolStack.pop();
			}
			symbolStack.pop();  //清掉(
		}
		else if (getWeight(infix.front()) >= 3)
		{
			int weight = getWeight(infix.front());
			while (!symbolStack.empty() && weight < getWeight(symbolStack.top()))
			{
				postfix.push_back(symbolStack.top());
				symbolStack.pop();
			}
			symbolStack.push(infix.front());
		}
		else if (getWeight(infix.front()) >= 1)
		{
			int weight = getWeight(infix.front());
			while (!symbolStack.empty() && weight <= getWeight(symbolStack.top()))
			{
				postfix.push_back(symbolStack.top());
				symbolStack.pop();
			}
			symbolStack.push(infix.front());
		}
		infix.pop();
	}
	while (!symbolStack.empty())
	{
		postfix.push_back(symbolStack.top());
		symbolStack.pop();
	}

}

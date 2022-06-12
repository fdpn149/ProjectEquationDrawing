#include "parser.h"
#include <stack>
using std::stack;
using std::stod;
using std::to_string;

bool Parser::isVarNum(string str)
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

bool Parser::isOperator(string str)
{
	if (!isalnum(str.at(0)))  //若不是英數
		return true;
	if (str == "sin" || str == "cos")  //若是sin/cos
		return true;

	return false;
}

Parser::Parser()
{

}

string Parser::getVarName(string input)
{
	if (input.empty() || !std::isalpha(input.at(0))) return "";  //若輸入的第1字不是英文

	string v1 = "";  //開頭的變數名稱

	//找等號
	for (int i = 1; i < input.size() - 1; i++)
	{
		if (!isalnum(input.at(i)))
		{
			if (input.at(i) == '=')  //若遇到=
			{
				v1 = input.substr(0, i);
				break;
			}
			else
				return "";
		}
	}
	if (v1.empty() || v1 == "sin" || v1 == "cos") return "";  //若v1為空字串或sin/cos

	return v1;
}

string Parser::parseInput(string input, Storage& storage, int nowRow)
{
	string v1 = getVarName(input);  //將v1設為等號前的變數名稱
	int eq_pos = input.find("=");  //等號的位置

	if (v1 != "y" && findVariable(Storage::graphs.rbegin() + Storage::graphs.size() - nowRow - 1,
		Storage::graphs.rend(), v1) != Storage::graphs.rend()) return "";  //變數重複定義

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
					return "";
				i++;
			}
			if (i - 1 >= 0 && input.at(i - 1) == '.')
				return "";

			now_code = 8;
			next_code = 398;

			string str = input.substr(from, i - from);  //截取
			i--;

			storage.infix.push(str);
			continue;
		}
		//英文(sin/cos=16, 其它=64)
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
					return "";
				}
			}
			//變數判斷

			int from = i;
			//將i移到不是變數的地方
			while (i < input.length() && std::isalnum(input.at(i)))
				i++;
			string name = input.substr(from, i - from);  //截取變數名稱
			i--;

			if (name == "y")
				return "";

			auto variable_it = findVariable(Storage::graphs.rbegin() + Storage::graphs.size() - nowRow - 1,
				Storage::graphs.rend(), name);

			//若變數與等號前的變數相同 或 找不到變數
			if (name == v1 || (name != "x" && variable_it == Storage::graphs.rend()))
				return "";

			if ((*variable_it)->status == -1) return "";

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

			if (par_count < 0) return "";  //判斷是否多括

			now_code = 256;
			next_code = 262;
			storage.infix.push(")");
			continue;
		}

		return "";
	}

	if (par_count > 0)
		return "";



	if ((now_code & 488) == 0)
		return "";


	return v1;
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
		if (isVarNum(infix.front()))
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

double Parser::calculate(double x, vector<Graph*>::reverse_iterator rbegin, vector<Graph*>::reverse_iterator rend)
{
	if((*rbegin)->status == -1) throw calculate_error("cannot find variable");

	vector<string> postfix = (*rbegin)->postfix;

	for (int i = 0; i < postfix.size(); i++)
	{
		string& now = postfix.at(i);
		if (!isOperator(now))  //若不是運算符號
		{
			if (now == "x")
				now = to_string(x);
			else if (!isdigit(now.at(0)))  //判斷是否是變數
			{
				auto rit = findVariable(rbegin, Storage::graphs.rend(), now);
				if (rit != Storage::graphs.rend())
				{
					double num = calculate(x, rit, rend);  //先計算變數的數值
					now = to_string(num);  //將該項取代成數值
				}
				else
				{
					(*rbegin)->status = -1;
					throw calculate_error("cannot find variable");
				}
			}
		}
		else  //是運算符號
		{
			double pre1 = 0, pre2 = 0;  //前兩項的數值
			if (i - 1 >= 0) pre1 = stod(postfix.at(i - 1));
			if (i - 2 >= 0) pre2 = stod(postfix.at(i - 2));

			if (now.size() > 1 && i - 1 >= 0)  //sin/cos/負號
			{
				if (now == "sin")
					now = to_string(std::sin(pre1));
				else if (now == "cos")
					now = to_string(std::cos(pre1));
				else
					now = to_string(-pre1);
				i--;
				postfix.erase(postfix.begin() + i);
			}
			else if (i - 2 >= 0)
			{
				if (now == "+")
					now = to_string(pre2 + pre1);
				else if (now == "-")
					now = to_string(pre2 - pre1);
				else if (now == "*")
					now = to_string(pre2 * pre1);
				else if (now == "/")
					now = to_string(pre2 / pre1);
				else if (now == "^")
					now = to_string(std::pow(pre2, pre1));

				if (!isdigit(now.at(now.length() - 1)))  //若最後一字不為數字(代表計算出問題)
					throw calculate_error(now);

				i -= 2;
				postfix.erase(postfix.begin() + i, postfix.begin() + i + 2);
			}
		}
	}
	return stod(postfix.at(0));
}

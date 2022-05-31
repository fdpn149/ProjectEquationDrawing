#include "parser.h"

Parser::Parser()
{

}

int Parser::parseInput(string input, Storage& storage)
{
	if (!std::isalpha(input.at(0))) return -1;  //若輸入的第1字不是英文，回傳-1

	string v1 = "";  //開頭的變數名稱
	int eq_pos;  //等號的位置

	//找等號
	for (eq_pos = 0; eq_pos < input.size() - 1; eq_pos++)
	{
		if (input.at(eq_pos) == '=')  //若遇到=
		{
			v1 = input.substr(0, eq_pos);
			break;
		}
	}
	if (v1.empty() || v1 == "sin" || v1 == "cos") return -1;  //若v1為空字串，回傳-1

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
			continue;
		}
		//若是負號或減號
		if ((next_code & 2) >= 1 && input.at(i) == '-')
		{
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
			continue;
		}
		//整數
		if ((next_code & 8) >= 1 && std::isdigit(input.at(i)))
		{
			now_code = 8;
			next_code = 398;
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

			now_code = 64;
			next_code = 262;
			continue;
		}
		//小數點
		if ((next_code & 128) >= 1 && input.at(i) == '.')
		{
			now_code = 128;
			next_code = 8;
			continue;
		}
		//右括號
		if ((next_code & 256) >= 1 && input.at(i) == ')')
		{
			par_count--;

			if (par_count < 0) return -1;  //判斷是否多括

			now_code = 256;
			next_code = 262;
			continue;
		}

		return -1;
	}

	if (par_count > 0)
		return -1;

	if((now_code & 488) >= 1)
		return 0;

	return -1;
}

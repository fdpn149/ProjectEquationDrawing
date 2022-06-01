#include "parser.h"
vector<string> Parser::part_input;
Parser::Parser()
{

}

int Parser::parseInput(string input, Storage& storage)
{
	if (!std::isalpha(input.at(0))) return -1;  //�Y��J����1�r���O�^��A�^��-1

	string v1 = "";  //�}�Y���ܼƦW��
	int eq_pos;  //��������m

	//�䵥��
	for (eq_pos = 0; eq_pos < input.size() - 1; eq_pos++)
	{
		if (input.at(eq_pos) == '=')  //�Y�J��=
		{
			v1 = input.substr(0, eq_pos);
			break;
		}
	}
	if (v1.empty() || v1 == "sin" || v1 == "cos") return -1;  //�Yv1���Ŧr��A�^��-1


	int par_count = 0;  //�p��W�U�A����
	int next_code = 123;  //�U�@�Ӧ��Ħr�����N�X #��l��(-0axs
	int now_code = 0;

	for (int i = eq_pos + 1; i < input.size(); i++)
	{
		//�Y�O���A��
		if ((next_code & 1) >= 1 && input.at(i) == '(')
		{
			par_count++;
			now_code = 1;
			next_code = 123;
			part_input.push_back("(");
			continue;
		}
		//�Y�O�t���δ
		if ((next_code & 2) >= 1 && input.at(i) == '-')
		{
			now_code = 2;
			next_code = 121;
			part_input.push_back("-");
			continue;
		}
		//�[������
		if ((next_code & 4) >= 1 && (input.at(i) == '+' || input.at(i) == '*'
			|| input.at(i) == '/' || input.at(i) == '^'))
		{
			now_code = 4;
			next_code = 121;
			part_input.push_back(string(1, input.at(i)));
			continue;
		}
		//�Ʀr
		if ((next_code & 8) >= 1 && std::isdigit(input.at(i)))
		{
			int from = i;

			bool flag = false;
			//�Ni���줣�O�Ʀr���a��
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

			string str = input.substr(from, i - from);  //�I��
			i--;

			part_input.push_back(str);
			continue;
		}
		//�^��(sin/cos=16, x=32, �䥦=64)
		if ((next_code & 112) >= 1 && std::isalpha(input.at(i)))
		{
			//sin/cos�P�_
			if (input.substr(i, 3) == "sin" || input.substr(i, 3) == "cos")
			{
				if ((next_code & 16) >= 1 && i + 3 < input.length() && input.at(i + 3) == '(')  //�Ysin/cos���U�Ӧr��(
				{
					part_input.push_back(input.substr(i, 3));
					i += 2;
					now_code = 16;
					next_code = 1;
					continue;
				}
				else if (i + 3 < input.length() && !std::isalpha(input.at(i + 3)) && !std::isdigit(input.at(i + 3)))  //�Y�U�Ӧr�����^��μƦr(���O�ܼ�)
				{
					return -1;
				}
			}
			//x�P�_
			else if (input.at(i) == 'x')
			{
				//�Y����ª�x
				if ((next_code & 32) >= 1 && (i + 1 >= input.length() || !std::isalnum(input.at(i + 1))))
				{
					now_code = 32;
					next_code = 262;
					part_input.push_back("x");
					continue;
				}
			}

			//�䥦�ܼƧP�_//
			int from = i;

			//�Ni���줣�O�ܼƪ��a��
			while (i < input.length() && std::isalnum(input.at(i)))
				i++;
			string name = input.substr(from, i - from);  //�I���ܼƦW��
			i--;

			//TODO:�M���ܼƬO�_�s�b

			part_input.push_back(name);
			now_code = 64;
			next_code = 262;
			continue;
		}
		//�p���I
		if ((next_code & 128) >= 1 && input.at(i) == '.')
		{
			now_code = 128;
			next_code = 8;
			part_input.push_back(".");
			continue;
		}
		//�k�A��
		if ((next_code & 256) >= 1 && input.at(i) == ')')
		{
			par_count--;

			if (par_count < 0) return -1;  //�P�_�O�_�h�A

			now_code = 256;
			next_code = 262;
			part_input.push_back(")");
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

#include"Convert.h"
#include"table.h"

std::string IntTo16(unsigned int x)
{
	char a[10] = {};
	sprintf(a, "%08X", x);
	return a;
}
std::string IntToStr(unsigned int x)
{
	char a[10] = {};
	sprintf(a, "%d", x);
	return a;
}
int StrToInt(std::string str)
{
	if (str.empty())
		return 0;
	int i = 0, label = 1;
	int result = 0;
	int N = str.size();
	if (str[0] == '0')
	{
		if (str[1] == 'x' || str[1] == 'X') // 16进制
		{
			for (i = 2; i < N; i++)
				result = result * 16 + RegDecoder::Decoder2[str[i]];
		}
		else		//8进制
		{
			for (i = 1; i < N; i++)
				result = result * 8 + (str[i] - '0');
		}
	}
	else
	{
		if (str[0] == '-')
		{
			i = 1; label = -1;
		}
		for (; i < N; i++)
			result = result * 10 + (str[i] - '0');
		result *= label;
	}
	return result;
}

std::string CharToBin(char c)
{
	unsigned char cc = c;
	std::string str;
	for (int i = 0; i < 8; i++)
	{
		if (cc >> (7 - i) & 1)
			str.push_back('1');
		else str.push_back('0');
	}
	return str;
}
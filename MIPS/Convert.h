#ifndef _CONVERT_H_
#define _CONVERT_H_
#include<string>

std::string IntTo16(unsigned int);			//int类型变为16进制std::string
std::string IntToStr(unsigned int);
int StrToInt(std::string);					//std::string翻译为int型整数
std::string CharToBin(char);


#endif // !_CONVERT_H_

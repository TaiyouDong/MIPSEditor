#ifndef _CONVERT_H_
#define _CONVERT_H_
#include<string>

std::string IntTo16(unsigned int);			//int���ͱ�Ϊ16����std::string
std::string IntToStr(unsigned int);
int StrToInt(std::string);					//std::string����Ϊint������
std::string CharToBin(char);


#endif // !_CONVERT_H_

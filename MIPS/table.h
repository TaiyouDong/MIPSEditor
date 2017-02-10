#ifndef _TABLE_H_
#define _TABLE_H_

#include <map>
#include <set>
#include <string>
#include <QString>

class RegDecoder {
public:
	static std::map<std::string, unsigned int> REG_num;

	static  std::string REG_name[32];

	static std::map<std::string, unsigned int> OptionCode;

	static std::map<std::string, unsigned int> FunCode;

	static std::map<char, std::string> Decoder;

	static std::map<char, unsigned int> Decoder2;

	static std::map<std::string, char> Decoder3;

	static std::map<int, std::string> InFunCode;

	static std::map<int, std::string> InOptCode;

	static std::set<std::string> DataOpt;
};



#endif // !_TABLE_H_

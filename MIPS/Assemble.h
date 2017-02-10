#ifndef _ASSEMBLE_H_
#define _ASSEMBLE_H_
#include<QString>
#include<string>
#include<vector>
#include<map>
#include<set>

class AssembleOpt {
private:
	unsigned int BaseAddress = 0x00001000;
	std::map<std::string, unsigned int> LabelAddress;
	std::vector<unsigned int> DataBlock;
	unsigned int place ;
	unsigned int BaseDataAddress = 0x00002000;
	unsigned int DataPlace;

public:
	
	AssembleOpt();
	~AssembleOpt();
	void AsmToBin(QString&, QString&, QString &);
	unsigned int  Trans_line(std::string&, std::string&);
	bool OnlyHavelabel(const std::string& text);
	std::string getLabel(std::string &);
	std::string getOper(std::string&);
	std::string getBranch(const std::string & text);
	unsigned int ALURType(std::string& text);
	unsigned int ALUIType(std::string& text);
	unsigned int BranchType(std::string&);
	unsigned int JType(std::string&);
	unsigned int LSType(std::string&);
	void getLabelAddress(std::string&,QString&);	//先扫描一遍字符串得到所有标签的位置
	static std::string::size_type Findchar(const std::string&, int, const std::string);
	static std::string::size_type Findchar(const std::string&, int, char);
	void ReplaceMove(std::string& str);			//替换move指令
	void ReplaceLi(std::string& str);			//替换li指令
	void ReplaceLa(std::string& str);
	void Replace(std::string& str);				//替换伪代码
	void DataOpt1(std::string, std::string, QString&);
	void DataOpt2(std::string, std::string, QString&);
};






#endif // !_ASSEMBLE_H_

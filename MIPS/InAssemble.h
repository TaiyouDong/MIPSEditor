#ifndef _INASSEMBLE_H_
#define _INASSEMBLE_H_
#include<QString>

class InAssembleOpt
{
private:
	int RegValue[32];	//����Ĵ����ڲ����ݵ�ֵ
	unsigned int BaseAddress = 0x0001000;
	unsigned int place;

public:
	void BinToAsm(QString&, QString&, QString&);
	std::string HexToAsm(std::string&,std::string&);
};

#endif // !_INASSEMBLE_H_
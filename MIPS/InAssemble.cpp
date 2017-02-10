#include"InAssemble.h"
#include"Assemble.h"
#include"Convert.h"
#include"table.h"


void InAssembleOpt::BinToAsm(QString&HexCode, QString&asmtext, QString&Address)
{
	std::string plaintext = HexCode.toStdString();
	place = BaseAddress - 4;
	std::string asmcode, address, tem, line;
	int i1 = 0, i2 = 0;
	do {
		i1 = i2;
		i2 = AssembleOpt::Findchar(plaintext, i1, '\n');
		tem = { plaintext.begin() + i1,plaintext.begin() + i2 };
		line = HexToAsm(tem,address);
		asmcode = asmcode + "    " + line + '\n';
		if (i2 < plaintext.size())
			i2++;
	} while (i2 < plaintext.size());
	asmcode.pop_back();
	address.pop_back();
	asmtext = QString::fromStdString(asmcode);
	Address = QString::fromStdString(address);
}

std::string InAssembleOpt::HexToAsm(std::string& text, std::string&address)
{
	place += 4;
	std::string tem = IntTo16(place), str;
	address = address + tem + '\n';
	text = "0x" + text;
	unsigned int number = StrToInt(text);
	if (number == 0)
		return "nop";
	else if (number == 0xc)
		return "syscall";
	int option = (number & 0xfc000000) >> 26;
	int function = number & 0x0000003f;
	int rs = (number & 0x03E00000) >> 21;
	int rt = (number & 0x001F0000) >> 16;
	int rd = (number & 0x0000F800) >> 11;
	if (option == 0)		//R型
	{
		if (function == 8) // jr
		{
			str = "jr " + RegDecoder::REG_name[rs];
		}
		else if (9 == function)//jalr
		{
			str = "jalr " + RegDecoder::REG_name[rs] + ", " + RegDecoder::REG_name[rd];
		}
		else if (function >= 0x18 && function <= 0x1b)
		{
			str = RegDecoder::InFunCode[function] + " " + RegDecoder::REG_name[rs] + ", " + RegDecoder::REG_name[rt];
		}
		else if (function == 0 || function == 2)//sll srl
		{
			int shamt = (number & 0x000007C0) >> 6;
			std::string shamtstr = IntToStr(shamt);
			shamtstr = shamtstr;
			str = RegDecoder::InFunCode[function] + " " + RegDecoder::REG_name[rd] + ", " +
				RegDecoder::REG_name[rt] + ", " + shamtstr;
		}
		else
		{
			str = RegDecoder::InFunCode[function] + " " + RegDecoder::REG_name[rd] + ", "+ 
					RegDecoder::REG_name[rs]+ ", " + RegDecoder::REG_name[rt];
		}
	}
	else if(option >= 0x08 && option <= 0x0e) // ALUi
	{
		int imm = (number & 0x0000FFFF);
		if (imm >= 0x8000)
			imm += 0xFFFF0000;
		std::string immstr = IntToStr(imm);
		immstr =immstr;
		str = RegDecoder::InOptCode[option] + " " + RegDecoder::REG_name[rt] + ", " + RegDecoder::REG_name[rs] +", "+ immstr;
	}
	else if (option == 1 || (option >=4 && option <= 7))//分支指令
	{
		int offset = (number & 0x0000FFFF);
		if (offset >= 0x8000)
			offset += 0xFFFF0000;
		//offset *= 4;
		std::string offstr = IntToStr(offset);
		offstr =offstr;
		if (option == 4 || option == 5)
		{
			str = RegDecoder::InOptCode[option] + " " + RegDecoder::REG_name[rs] + ", " + RegDecoder::REG_name[rt] + ", " + offstr;
		}
		else if (option == 6 || option == 7)
		{
			str = RegDecoder::InOptCode[option] + " " + RegDecoder::REG_name[rs] + ", " + offstr;
		}
		else if (option == 1)
		{
			std::string option1;
			if (rt == 1) option1 = "bgez";
			else if (rt == 0x11) option1 = "bgezal";
			else if (rt == 0x10) option1 = "bltzal";
			else if (rt == 0) option1 == "bltz";
			str = option1 + " " + RegDecoder::REG_name[rs] + ", " + offstr;
		}
	}
	else if (option >= 0x20 && option <= 0x2e)
	{
		int offset = (number & 0x0000FFFF);
		if (offset >= 0x8000)
			offset += 0xFFFF0000;
		std::string offstr = IntToStr(offset);
		offstr = offstr;
		str = RegDecoder::InOptCode[option] + " " + RegDecoder::REG_name[rt] + ", " + offstr + "(" + RegDecoder::REG_name[rs] + ")";
	}
	else if (option == 2 || option == 3)
	{
		int target = (number & 0x03FFFFFF) << 2;
		std::string tarstr = IntTo16(target);
		tarstr = "0x" + tarstr;
		str = RegDecoder::InOptCode[option] + " " + tarstr;
	}
	return str;
}
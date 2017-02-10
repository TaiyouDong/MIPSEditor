#include"Assemble.h"
#include"Convert.h"
#include<algorithm>
#include"table.h"
#include<QString>
#include<string>
#include<cctype>
#include<cstring>
#include<map>
#include<set>


AssembleOpt::AssembleOpt()
{
	place = 0;
}
AssembleOpt::~AssembleOpt()
{
	LabelAddress.clear();
}

void AssembleOpt::AsmToBin(QString& plaintext,QString& Code,QString &Address)
{
	place = BaseAddress;
	std::string text = plaintext.toStdString();	//QString转化为string
	while(isspace(text[text.size() - 1]))
		text.pop_back();
	std::string BinCode, tem, address;
	unsigned int binText;
	std::string::iterator it1 , it2 = text.begin();
	do
	{
		it1 = it2;
		while (it2 != text.end() && *it2 != '\n')
			it2++;
		
		tem = { it1,it2 };				//tem 不含 '\n'
		binText = Trans_line(tem,address);
		if (binText != 0xffffffff)
		{
			std::string tem = IntTo16(binText);
			BinCode = BinCode + tem + '\n';
		}
		if (it2 != text.end())
			it2++;

	} while (it2 != text.end());
	BinCode.pop_back();
	Code = QString::fromStdString(BinCode);
	address.pop_back();			//删除最后多余的换行符
	Address = QString::fromStdString(address);
}

unsigned int AssembleOpt::Trans_line(std::string& text,std::string& address)
{
	unsigned int result = 0;
	if (text.find(':') != std::string::npos) //含有一个标签
	{
		std::string label = getLabel(text);
		if (OnlyHavelabel(text))
		{
			return 0xffffffff;
		}
		else								//含有标签和语句
		{									//先把标签去掉，剩下处理和普通情况一样
			int i = text.find(':');
			i++;
			if (label == "DataAddre")
			{
				text = { text.begin() + i,text.begin() + Findchar(text,i," \n;") };
				text = "0x" + text;
				unsigned int dataddr = StrToInt(text);
				DataBlock.push_back(dataddr);
				return 0xffffffff;
			}
			for (; i < text.size(); i++)
				if (!isspace(text[i]))
					break;
			text = { text.begin() + i,text.end() };
		}
	}				
	std::string operation = getOper(text);
	if (operation == "db" || operation == "dw" || operation == "dd" || operation == "resb" || operation == "resw" || operation == "resd"){ }
	else
	{
		std::string tem = IntTo16(place);
		address = address + tem + '\n';
		place += 4;
	}
	if (RegDecoder::OptionCode.find(operation) != RegDecoder::OptionCode.end())		//如果能找到操作符
	{
		if (RegDecoder::OptionCode[operation] == 0)						//ALUr运算
			if (operation == "jr")
			{
				int i1 = 0, i2;
				std::string rs, rt, rd;
				i1 = text.find('$');i1++;
				i2 = Findchar(text, i1, ", ;");
				rs = {text.begin()+i1,text.begin()+i2};
				result = RegDecoder::FunCode[operation] + (RegDecoder::REG_num[rs] << 21);
			}
			else if (operation == "jalr")
			{
				std::string rs, rd;
				int i1, i2, N = text.size();
				i1 = text.find('$');i1++; 
				i2 = Findchar(text, i1, ", ");
				rs = { text.begin() + i1,text.begin() + i2 };
				i1 = i2;
				while (text[i1] != '-' && !isdigit(text[i1]))
					i1++;
				i2 = i1;
				i2 = Findchar(text, i1, "; \n");
				rd = { text.begin() + i1,text.begin() + i2 };
				result = 9 + (RegDecoder::REG_num[rd] << 11) + (RegDecoder::REG_num[rs] << 21);
			}
			else result = ALURType(text);
		else if (RegDecoder::OptionCode[operation] >= 8 && RegDecoder::OptionCode[operation] <= 15)	//ALUi运算
			if (operation == "lui")
			{
				int i1 = 0, i2 = 0, N = text.size(), num;
				std::string rt, immi;
				while (text[i1] != '$')
					i1++;
				i1++; i2 = i1;
				while (text[i2] != ',' && text[i2] != ' ')
					i2++;
				rt = { text.begin() + i1,text.begin() + i2 };
				i1 = i2;
				while (text[i1] != '-' && !isdigit(text[i1]))
					i1++;
				i2 = i1;
				while (i2 < N && text[i2] != ' ' && text[i2] != '\n' && text[i2] != ';')
					i2++;
				immi = { text.begin() + i1,text.begin() + i2 };
				num = StrToInt(immi);
				result = (num & 0x0000ffff) + (RegDecoder::REG_num[rt] << 16) + (RegDecoder::OptionCode[operation] << 26);
			}
			else result = ALUIType(text);
		else if (RegDecoder::OptionCode[operation] >= 1 && RegDecoder::OptionCode[operation] <= 7)	//分支和跳转指令
		{
			if (RegDecoder::OptionCode[operation] == 2 || RegDecoder::OptionCode[operation] == 3)	//跳转指令，绝对地址
			{
				result = JType(text);
			}
			else result = BranchType(text);		//分支指令，pc相对寻址
		}
		else if (RegDecoder::OptionCode[operation] >= 0x20 && RegDecoder::OptionCode[operation] <= 0x2e)	//取数保存指令
		{
			result = LSType(text);
		}
	}
	else if (operation == "nop")
		result = 0;
	else if (operation == "syscall")
		result = 0x0000000c;
	else if (RegDecoder::DataOpt.find(operation) != RegDecoder::DataOpt.end())//对数据区的操作            
	{
		if(operation == "db" || operation == "dw" || operation == "dd" || operation == "resb" || operation == "resw" || operation == "resd")
			return 0xffffffff;
	}

	return result;
}



unsigned int AssembleOpt::ALURType(std::string& text)
{
	std::string Oper = getOper(text);
	int i1 = 0, i2;
	std::string rs, rt, rd;
	i1 = Findchar(text,0,'$'); i1++;
	i2 = Findchar(text, i1, " ,");
	rd = { text.begin()+i1,text.begin()+i2 };
	i1 = i2;
	i1 = Findchar(text, i2, '$'); i1++;
	i2 = Findchar(text, i1, " ,;");
	rs = { text.begin() + i1,text.begin() + i2 }; // sll -> rt
	i1 = i2;
	if (RegDecoder::FunCode[Oper] >= 0x18 && RegDecoder::FunCode[Oper] <= 0x1b)
	{
		rt = rs; rs = rd;
	}
	else
	{
		while (i1 != text.size() && text[i1] != '$' && !isdigit(text[i1]))
			i1++;
		if (text[i1] == '$')		//如果是sll 或者 srl的话，最后一个可能是数字
			i1++;
		i2 = Findchar(text, i1, " ';");
		rt = { text.begin() + i1,text.begin() + i2 };
	}
	unsigned int result = 0;
	if (Oper == "sll")
	{
		std::string h = rs;
		rs = rt; rt = h; h = rs;
		result = RegDecoder::FunCode[Oper] + (StrToInt(h) << 6) + (RegDecoder::REG_num[rd] << 11) + (RegDecoder::REG_num[rt] << 16);
	}
	else if (Oper == "srl")
	{
		std::string h = rs;
		rs = rt; rt = h; h = rs;
		result = RegDecoder::FunCode[Oper] + (StrToInt(h) << 6) + (RegDecoder::REG_num[rd] << 11) + (RegDecoder::REG_num[rt] << 16);
	}
	else if (RegDecoder::FunCode[Oper] >= 0x18 && RegDecoder::FunCode[Oper] <= 0x1b)//乘法或除法
	{
		result = RegDecoder::FunCode[Oper] + (RegDecoder::REG_num[rt] << 16) + (RegDecoder::REG_num[rs] << 21);
	}
	else
	{
		result = RegDecoder::FunCode[Oper] + (RegDecoder::REG_num[rd] << 11) + (RegDecoder::REG_num[rt] << 16) + (RegDecoder::REG_num[rs] << 21);
	}
	return result;
}
unsigned int AssembleOpt::ALUIType(std::string& text)
{
	std::string Oper = getOper(text);
	std::string rs, rt, immi;
	int num;
	unsigned int result;
	int i1 = 0, i2 = 0, N = text.size();
	i1 = text.find('$');i1++; 
	i2 = Findchar(text, i1, ", ");
	rt = { text.begin() + i1,text.begin() + i2 };
	i1 = i2;
	i1 = Findchar(text,i2,'$'); i1++;
	i2 = Findchar(text, i1, ", ");
	rs = { text.begin() + i1,text.begin() + i2 };
	i1 = i2;
	while (text[i1] != '-' && !isdigit(text[i1]))
		i1++;
	i2 = Findchar(text, i1, " \n;");
	immi = { text.begin() + i1,text.begin() + i2 };
	num = StrToInt(immi);
	result = (num & 0x0000ffff) + (RegDecoder::REG_num[rt] << 16) + (RegDecoder::REG_num[rs] << 21) + (RegDecoder::OptionCode[Oper] << 26);
	return result;
}
unsigned int AssembleOpt::BranchType(std::string& text)
{
	int i1, i2, N = text.size();
	std::string Oper = getOper(text);
	std::string rs, rt;
	unsigned int result, x;
	std::string branch = getBranch(text);
	int offset;
	if(LabelAddress.find(branch) != LabelAddress.end())
		offset = (LabelAddress[branch]-place)/4;
	else offset = StrToInt(branch);
	i1 = Findchar(text, 0, '$');
	i1++; 
	i2 = Findchar(text, i1, ", ");
	rs = { text.begin() + i1,text.begin() + i2 };
	if (RegDecoder::OptionCode[Oper] == 4 || RegDecoder::OptionCode[Oper] == 5)
	{
		i1 = Findchar(text, i2, '$');
		i1++;
		i2 = Findchar(text, i1, ", ");
		rt = { text.begin() + i1,text.begin() + i2 };
		x = RegDecoder::REG_num[rt];
	}
	else
	{
		if (Oper == "bgez") x = 1;
		else if (Oper == "bgezal") x = 0x11;
		else if (Oper == "bgtz") x = 0;
		else if (Oper == "blez") x = 0;
		else if (Oper == "bltzal") x = 0x10;
		else if (Oper == "bltz") x = 0;
	}
	result = (offset & 0x0000ffff) + (x << 16) + (RegDecoder::REG_num[rs] << 21) + (RegDecoder::OptionCode[Oper] << 26);
	return result;
}
unsigned int AssembleOpt::JType(std::string& text)
{
	unsigned int result;
	int address;
	std::string Oper = getOper(text);
	std::string branch = getBranch(text);
	if(LabelAddress.find(branch) != LabelAddress.end())
		address = (LabelAddress[branch] & 0x00ffffff);
	else address = StrToInt(branch);
	result = (address>>2) + (RegDecoder::OptionCode[Oper] << 26);
	return result;
}
unsigned int AssembleOpt::LSType(std::string& text)
{
	std::string Oper = getOper(text);
	unsigned int result;
	int i1, i2, N = text.size(), offset;
	std::string rs, rt, immi;
	i1 = Findchar(text, 0, '$');
	i1++; 
	i2 = Findchar(text, i1, ", ");
	rt = { text.begin() + i1,text.begin() + i2 };
	i1 = i2;
	while (!isdigit(text[i1]))
		i1++;
	i2 = Findchar(text, i1, "( ");
	immi = { text.begin() + i1,text.begin() + i2 };
	i1 = Findchar(text, i2, '$');
	i1++;
	i2 = Findchar(text, i1, ", )");
	rs = { text.begin() + i1,text.begin() + i2 };
	offset = StrToInt(immi);
	result = (offset & 0x0000ffff) + (RegDecoder::REG_num[rt] << 16) + (RegDecoder::REG_num[rs] << 21) + (RegDecoder::OptionCode[Oper] << 26);
	return result;
}

bool AssembleOpt::OnlyHavelabel(const std::string& text)
{
	int x = text.find(':');
	int n = text.size(), i;
	for (i = x + 1; i < n; i++)
	{
		if (!isspace(text[i]))
			break;
	}
	return i >= n;
}

std::string AssembleOpt::getLabel( std::string & text)
{
	std::string::iterator it1 = text.begin(),it2 ;
	while (it1 !=text.end() && isspace(*it1))
		it1++;
	it2 = it1;
	while (it2!=text.end() && *it2 != ':')
		it2++;
	return{ it1,it2 };
}
std::string AssembleOpt::getOper(std::string& str)
{
	std::string::iterator it1 = str.begin(), it2;
	while (it1 != str.end() && isspace(*it1))
		it1++;
	it2 = it1;
	while (it2 != str.end() && *it2 != ' ' && *it2 != '$')
		it2++;
	return{ it1,it2 };
}
void AssembleOpt::getLabelAddress(std::string& text,QString& DataContain)//数据区的添加必须在代码段的后面
{
	DataContain.clear();
	LabelAddress.clear();
	std::string tem, label;
	int i1 = 0, i2 = 0, N = text.size();
	int i = 0;
	int mode = 0;
	N = text.size();
	int address = BaseAddress;
	do {
		i1 = i2;
		while (i2 < N && text[i2] != '\n')
			i2++;
		tem = { text.begin() + i1,text.begin() + i2 };
		if (tem.find(':') != std::string::npos)
		{
			label = getLabel(tem);
			if (label == "DataAddre")//发现是数据区就跳出循环
			{
				mode = 1;
				break;
			}
			LabelAddress[label] = address;
			if (!OnlyHavelabel(tem))
				if (tem.find("la") != std::string::npos)
				{
					address += 8;
				}
				else address += 4;
		}
		else
		{
			if (tem.find("la") != std::string::npos)
			{
				address += 8;
			}
			else address += 4;
		}
		i2++;
	} while (i2 < N);

	if (mode == 1)
	{
		do{
			if (tem.find(':') != std::string::npos)
			{
				label = getLabel(tem);
				if (label == "DataAddre")
				{
					int l1 = tem.find(':'), l2;
					l1++;
					while (tem[l1] == ' ') l1++;
					l2 = Findchar(tem, l1, " ;\n");
					std::string s = { tem.begin() + l1,tem.begin() + l2 };
					s = "0x" + s;
					DataPlace = StrToInt(s);
					i2++;
					i1 = i2;
					while (i2 < N && text[i2] != '\n')
						i2++;
					tem = { text.begin() + i1,text.begin() + i2 };
					continue;
				}
				LabelAddress[label] = DataPlace;
				if (!OnlyHavelabel(tem)) //如果不是只含有标签，则把标签去掉执行下面的操作
				{
					i1 = tem.find(':');
					i1++;
					while (tem[i1] == ' ' || tem[i1] == '\t')
						i1++;
					tem = { tem.begin() + i1,tem.end() };
				}
			}
			std::string opt = getOper(tem);
			if (opt == "db" || opt == "dw" || opt == "dd")
			{
				int l1 = 0, l2 = 0;
				std::string str;
				l1 = tem.find('d');
				l1 += 2;
				while (tem[l1] == ' ') l1++;
				l2 = l1;
				while (l2 < tem.size())
				{
					l2 = Findchar(tem, l2, ", ;\n");
					str = { tem.begin() + l1,tem.begin() + l2 };
					DataOpt1(opt, str, DataContain);

					while ((l2 < tem.size())&&( tem[l2] == ' ' || tem[l2] == ',' || tem[l2] == ';' || tem[l2] == '\n'))
					l2++;
					l1 = l2;
				}
			}
			else if (opt == "resb" || opt == "resw" || opt == "resd")
			{
				int l1 = 0, l2 = 0;
				std::string str;
				l1 = tem.find('r');
				l1 += 4;
				while (tem[l1] == ' ') l1++;
				l2 = Findchar(tem, l1, " ,;\n");
				str = { tem.begin() + l1,tem.begin() + l2 };
				DataOpt2(opt, str, DataContain);
			}
			if (i2 >= N) break;
L1:
			i2++;
			i1 = i2;
			while (i2 < N && text[i2] != '\n')
				i2++;
			tem = { text.begin() + i1,text.begin() + i2 };
		} while (1);
	}
}

void AssembleOpt::DataOpt2(std::string opt, std::string data, QString& DataContain)
{
	int label;
	if (opt == "resb")
		label = 1;
	else if (opt == "resw")
		label = 2;
	else if (opt == "resd")
		label = 4;
	int n = StrToInt(data);
	n = n * label;
	for (int i = 1; i <= n; i++)
	{
		if (DataPlace % 4 == 0)
			DataContain += QString::fromStdString("\n" + IntTo16(DataPlace) + ": ");
		DataPlace += 1;
		DataContain += QString::fromStdString("00000000");
	}
}

void AssembleOpt::DataOpt1(std::string opt, std::string data,QString& DataContain)
{
	if (data[0] == '\'' || data[0] == '"')
	{
		int num;
		if (opt == "db")
			num = 1;
		else if (opt == "dw")
			num = 2;
		else if (opt == "dd")
			num = 4;
		for (int i = 1; i <= num; i++)
		{
			if (DataPlace % 4 == 0)
				DataContain += QString::fromStdString("\n" + IntTo16(DataPlace) + ": ");
			char c = data[i];
			std::string str = CharToBin(c);
			DataPlace += 1;
			DataContain += QString::fromStdString(str);
		}
	}
	else if (isdigit(data[0]))
	{
		if (data[1] == 'x' || data[1] == 'X')
		{
			data = { data.begin() + 2,data.end() };
			int n = data.size();
			for (int i = 0; i < n; i += 2)
			{
				if (DataPlace % 4 == 0)
					DataContain += QString::fromStdString("\n" + IntTo16(DataPlace) + ": ");
				DataContain += QString::fromStdString(RegDecoder::Decoder[data[i]] + RegDecoder::Decoder[data[i + 1]]);
				DataPlace += 1;
			}
		}
		else
		{
			unsigned int x = StrToInt(data);
			std::string tem = IntTo16(x);
			int n;
			if (opt == "db")
				n = 2;
			else if (opt == "dw")
				n = 4;
			else if (opt == "dd")
				n = 8;
			tem = { tem.end() - n,tem.end() };
			for (int i = 0; i < n; i += 2)
			{
				if (DataPlace % 4 == 0)
					DataContain += QString::fromStdString("\n" + IntTo16(DataPlace) + ": ");
				DataContain += QString::fromStdString(RegDecoder::Decoder[tem[i]] + RegDecoder::Decoder[tem[i+1]]);
				DataPlace += 1;
			}
		}
	}
}

std::string AssembleOpt::getBranch(const std::string & text)
{
	int N = text.size();
	int i1, i2;
	i2 = N - 1;
	while (text[i2] == ';' || text[i2] == ' ' || text[i2] == '\n')
		i2--;
	i1 = i2; i2++;
	while (text[i1] != ',' && text[i1] != ' ')
		i1--;
	i1++;
	return{ text.begin() + i1,text.begin() + i2 };
}

void AssembleOpt::Replace(std::string& str)		//替换伪代码
{
	ReplaceMove(str);
	ReplaceLi(str);
	ReplaceLa(str);
}
void AssembleOpt::ReplaceMove(std::string& str)
{
	std::string tem, rs, rd;
	int i1 = 0, i2, j1, j2, n;
	i1 = str.find("move", 0);
	while (i1 != std::string::npos)		//替换move
	{
		i2 = i1;
		while (i2 < str.size() && str[i2] != '\n')
			i2++;
		tem = { str.begin() + i1,str.begin() + i2 };
		str.erase(i1, i2 - i1);
		std::string newstr;
		j1 = j2 = 0; n = tem.size();
		j1 = tem.find('$');
		j1++;
		j2 = Findchar(tem, j1, ", ");
		rd = { tem.begin() + j1,tem.begin() + j2 };
		j1 = Findchar(tem, j2, '$');
		j1++;
		j2 = Findchar(tem, j1, " ,");
		rs = { tem.begin() + j1,tem.begin() + j2 };
		newstr = newstr + "addu $" + rd + ", $zero" + ", $" + rs;
		str.insert(i1, newstr);
		i1 = str.find("move", i1);
	}
}
void AssembleOpt::ReplaceLi(std::string& str)
{
	std::string tem, rs, rd,immi;
	int i1, i2, j1, j2, n;
	i1 = str.find("li", 0);
	while (i1 != std::string::npos)
	{
		i2 = i1;
		while (i2 < str.size() && str[i2] != '\n')
			i2++;
		tem = { str.begin() + i1,str.begin() + i2 };
		str.erase(i1, i2 - i1);
		std::string newstr;
		j1 = tem.find('$');
		j1++;
		j2 = Findchar(tem, j1, " ,");
		rd = { tem.begin() + j1,tem.begin() + j2 };
		j1 = j2;
		while (!isdigit(tem[j1]))
			j1++;
		j2 = Findchar(tem, j1, " ,");
		immi = { tem.begin() + j1,tem.begin() + j2 };
		newstr = newstr + "ori $" + rd + ", $zero, " + immi;
		str.insert(i1, newstr);
		i1 = str.find("li", i1);
	}
}
void AssembleOpt::ReplaceLa(std::string& str)
{
	std::string tem, rs, address;
	int number;
	int i1, i2, j1, j2;
	int high, low;
	i1 = str.find("la");
	while (i1 != std::string::npos)
	{
		i2 = i1;
		while (i2 < str.size() && str[i2] != '\n')
			i2++;
		tem = { str.begin() + i1,str.begin() + i2 };
		str.erase(i1, i2-i1);
		std::string newstr;
		j1 = tem.find('$'); j1++;
		j2 = Findchar(tem, j1, " ,");
		rs = { tem.begin() + j1,tem.begin() + j2 };
		j1 = j2;
		while (tem[j1] == ' ' || tem[j1] == ',')
			j1++;
		j2 = Findchar(tem, j1, " ,");
		address = { tem.begin() + j1,tem.begin() + j2 };
		if (LabelAddress.find(address) != LabelAddress.end())
		{
			number = LabelAddress[address];
		}
		else number = StrToInt(address);
		high = (number & 0xffff0000) >> 16;
		low = number & 0x0000ffff;
		std::string highstr = IntToStr(high), lowstr = IntToStr(low);
		newstr = "lui $at, " + highstr + "\n          ori $" + rs + ", $at, " + lowstr;
		str.insert(i1, newstr);

		i1 = str.find("la",i1);
	}
}



std::string::size_type AssembleOpt::Findchar(const std::string& text, int pos, const std::string ref)
{
	int i = pos;
	for (; i < text.size(); i++)
	{
		for (int j = 0; j < ref.size(); j++)
		{
			if (text[i] == ref[j])
				return i;
		}
	}
	return i;
}
std::string::size_type AssembleOpt::Findchar(const std::string& text, int pos, char c)
{
	int i = pos;
	for (; i < text.size(); i++)
		if (text[i] == c)
			return i;
	return i;
}


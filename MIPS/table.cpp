#include <map>
#include <string>
#include <set>
#include "table.h"


std::map<std::string, unsigned int> RegDecoder::REG_num = {
	{ "zero",0 } ,{ "at",1 } ,{ "v0", 2 } ,{ "v1", 3 } ,{ "a0", 4 } ,{ "a1", 5 } ,{ "a2", 6 } ,{ "a3", 7 } ,
	{ "t0",8 } ,{ "t1",9 } ,{ "t2",10 } ,{ "t3",11 } ,{ "t4",12 } ,{ "t5",13 } ,{ "t6",14 } ,{ "t7",15 } ,
	{ "s0",16 } ,{ "s1",17 } ,{ "s2",18 } ,{ "s3",19 } ,{ "s4",20 } ,{ "s5",21 } ,{ "s6",22 } ,{ "s7",23 } ,
	{ "t8",24 }  ,{ "t9",25 } ,{ "k0",26 } ,{ "k1",27 } ,{ "gp",28 } ,{ "sp",29 } ,{ "fp",30 } ,{ "ra",31 }
};
std::string RegDecoder::REG_name[32] = {
	"$zero" , "$at" , "$v0" , "$v1" , "$a0" , "$a1" , "$a2" , "$a3" ,
	"$t0"   , "$t1" , "$t2" , "$t3" , "$t4" , "$t5" , "$t6" , "$t7" ,
	"$s0"   , "$s1" , "$s2" , "$s3" , "$s4" , "$s5" , "$s6" , "$s7" ,
	"$t8"   , "$t9" , "$k0" , "$k1" , "$gp" , "$sp" , "$fp" , "$ra"
};
std::map<std::string, unsigned int> RegDecoder::OptionCode = {
	{ "add",0 },
	{ "addu",0 },
	{ "addi",8 },
	{ "addiu",9 },
	{ "and",0 },
	{ "andi",0xc },
	{ "div",0 },
	{ "divu",0 },
	{ "mult",0 },
	{ "multu",0 },
	{ "mul",0x1c },	//乘积，不带溢出位，将rs，rt的低32位存入rd
	{ "nor",0 },
	{ "xor",0 },
	{ "xori",0 },
	{ "or",0 },
	{ "ori",0xd },
	{ "sll",0 },		//逻辑左移
	{ "sllv",0 },		//逻辑左移变量
	{ "sra",0 },		//算术右移
	{ "srav",0 },		//算术右移变量
	{ "srl",0 },		//逻辑右移
	{ "srlv",0 },		//逻辑右移变量
	{ "sub",0 },
	{ "subu",0 },
	{ "lui",0xf },
	{ "slt",0 },
	{ "sltu",0 },
	{ "slti",0xa },
	{ "sltiu",0xb },
	{ "beq",4 },
	{ "bgez",1 },		//大于等于0
	{ "bgezal",1 },	//大于等于0并链接
	{ "bgtz",7 },		//大于0分支
	{ "blez",6 },		//小于等于0分支
	{ "blezal",1 },	//小于等于0分支并链接
	{ "bltz",1 },		//小于0分支
	{ "bne",5 },
	{ "j",2 },
	{ "jal",3 },
	{ "jalr",0 },
	{ "jr",0 },
	{ "lb",0x20 },	//取字节
	{ "lbu",0x24 },	//
	{ "lh",0x21 },	//取半字
	{ "lhu",0x25 },	//
	{ "lw",0x23 },	//取字
	{ "lwl",0x22 },	//取左半字
	{ "lwr",0x26 },	//取右半字
	{ "sb",0x28 },	//存字节
	{ "sh",0x29 },	//存半字
	{ "sw",0x2b },	//存字
	{ "swl",0x2a },	//存左半字
	{ "swr",0x2e }	//存右半字

};

std::map<std::string, unsigned int> RegDecoder::FunCode = {
	{ "add",0x20 },
	{ "addu",0x21 },
	{ "and",0x24 },
	{ "div",0x1a },
	{ "divu",0x1b },
	{ "mult",0x18 },
	{ "multu",0x19 },
	{ "mul",2 },
	{ "nor",0x27 },
	{ "or",0x25 },
	{ "sll",0 },
	{ "sllv",4 },
	{ "sra",3 },
	{ "srav",7 },
	{ "srl",2 },
	{ "srlv",6 },
	{ "sub",0x22 },
	{ "subu",0x23 },
	{ "xor",0x26 },
	{ "slt",0x2a },
	{ "sltu",0x2b },
	{ "jr",8 },
	{ "jalr",9 }

};

std::map<char, std::string> RegDecoder::Decoder = {
	{ '0',"0000" } ,{ '1',"0001" } ,{ '2',"0010" } ,{ '3',"0011" },
	{ '4',"0100" } ,{ '5',"0101" } ,{ '6',"0110" } ,{ '7',"0111" },
	{ '8',"1000" } ,{ '9',"1001" } ,{ 'a',"1010" } ,{ 'b',"1011" },
	{ 'c',"1100" } ,{ 'd',"1101" } ,{ 'e',"1110" } ,{ 'f',"1111" },
	{ 'A',"1010" } ,{ 'B',"1011" } ,
	{ 'C',"1100" } ,{ 'D',"1101" } ,{ 'E',"1110" } ,{ 'F',"1111" }
};
std::map<std::string, char> RegDecoder::Decoder3 = {
	{ "0000",'0' },{ "0001",'1' },{ "0010",'2' },{ "0011",'3' },
	{ "0100",'4' },{ "0101",'5' },{ "0110",'6' },{ "0111",'7' },
	{ "1000",'8' },{ "1001",'9' },{ "1010",'A' },{ "1011",'B' },
	{ "1100",'C' },{ "1101",'D' },{ "1110",'E' },{ "1111",'F' }

};

std::map<char, unsigned int> RegDecoder::Decoder2 = {
	{ '0',0 },{ '1',1 },{ '2',2 },{ '3',3 },
	{ '4',4 },{ '5',5 },{ '6',6 },{ '7',7 },
	{ '8',8 },{ '9',9 },{ 'a',10 },{ 'b',11 },
	{ 'c',12 },{ 'd',13 },{ 'e',14 },{ 'f',15 },
	{ 'A',10 },{ 'B',11 },
	{ 'C',12 },{ 'D',13 },{ 'E',14 },{ 'F',15 }
};

std::map<int, std::string> RegDecoder::InFunCode = {
	{ 0x20,"add" },
	{ 0x21,"addu" },
	{ 0x22,"sub" },
	{ 0x23,"subu"},
	{ 0x2A,"slt"},
	{ 0x2B,"sltu"},
	{ 0x24,"and"},
	{ 0x25,"or"},
	{ 0x26,"xor"},
	{ 0x27,"nor"},
	{ 0x0,"sll"},
	{ 0x2,"srl"},
	{ 0x3,"sra"},
	{ 0x18,"mult"},
	{ 0x19,"multu"},
	{ 0x1A,"div"},
	{ 0x1B,"divu"},
	{ 0x09,"jalr"},
	{ 0x08,"jr"},
	{ 0x0D,"break"},
	{ 0x0C,"syscall"}
};

std::map<int, std::string> RegDecoder::InOptCode = {
	{0x08,"addi"},
	{0x09,"addiu"},
	{0x0c,"andi"},
	{0x0d,"ori"},
	{0x0e,"xori"},
	{0x0f,"lui"},
	{0x0a,"slti"},
	{0x0b,"sltiu"},
	{0x04,"beq"},
	{0x07,"bgtz"},
	{0x06,"blez"},
	{0x05,"bne"},
	{0x02,"j"},
	{0x03,"jal"},
	{0x24,"lbu"},
	{0x21,"lh"},
	{0x25,"lhu"},
	{0x23,"lw"},
	{0x22,"lwl"},
	{0x26,"lwr"},
	{0x30,"ll"},
	{0x28,"sb"},
	{0x29,"sh"},
	{0x2b,"sw"},
	{0x2a,"swl"},
	{0x2e,"swr"}
};

std::set<std::string> RegDecoder::DataOpt = {
	"db" , "dw" , "dd" , "resb" , "resw" , "resd" , "equ"  
};
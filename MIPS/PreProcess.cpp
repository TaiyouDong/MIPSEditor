#include"Preprocess.h"

void justify(QString& S)			//预处理，删掉注释，空白的行
{
	S = " lw $a0, 0($sp)\n addiu $a1, $sp, 4\n addiu $a2, $a1, 4\n sll $v0, $a0, 2\n addu $a2, $a2, $v0\n jal main\n nop\n li $v0, 10\n syscall\n" + S;
	S.replace('\t', " ");
	S.replace("    ", " ");
	S.replace("   ", " ");
	S.replace("  ", " ");
	int N = S.size();
	int i = 0;
	while (i + 1 < N)				//每行开始的空格换成4个空格
	{
		if (S[i] == '\n')
		{
			if (S[i + 1] == ' ')
			{
				S.insert(i + 1, "   ");
				N += 3;
			}
			else
			{
				S.insert(i + 1, "    ");
				N += 4;
			}
		}
		i++;
	}
	DeleteNode(S);
	DeleteVoidLine(S);
	StandLabel(S);
	while (!S.isEmpty() && (S[0] == ' ' || S[0] == '\n'))//删除开头的空白符
		S.remove(0, 1);
	S.insert(0, "    ");
	//S.remove(S.length() - 1, 1);
}
void DeleteNode(QString&S)				//删除注释
{
	int N = S.size();
	int n1 = 0, n2 = 0;
	while (n2 < N)
	{
		while (n1 < N && S[n1] != '#' && S[n1] != '/')
			n1++;
		n2 = n1;
		while (n2 < N && S[n2] != '\n')
			n2++;
		if (n2 < N)
		{
			S.remove(n1, n2 - n1);
			N -= n2 - n1;
		}
	}
}
void DeleteVoidLine(QString& S)			//删除空行
{
	int n1 = 0, n2 = 0, i;
	int N = S.size();
	while (n2 < N && S[n2] != '\n')
		n2++;
	n1 = n2; n2++;
	while (n2 < N)
	{
		while (n2 < N && S[n2] != '\n')		//找到下一个换行符
			n2++;
		for (i = n1; i < n2; i++)
			if (S[i] != ' ' && S[i] != '\n') break;
		if (i == n2)						//n1 和 n2之间只有 ' ' 和 '\n'，即为空行（这样也可以删除两个连着的'\n'）
		{
			S.remove(n1, n2 - n1);
			N -= n2 - n1;					//更新N的值
			n2 = n1 + 1;
		}
		else
		{
			n1 = n2; n2++;
		}
	}
}
bool Isspace(const std::string& str)
{
	if (str.empty())
		return true;
	int i = 0, n = str.size();
	while (i<n && isspace(str[i]))
		i++;
	return i == n;
}
void StandLabel(QString&S)
{
	int N = S.size();
	int n1 = 0, n2 = 0, i = 0;
	while (n2 < N)
	{
		while (n2 < N && S[n2] != '\n')
		{
			n2++;
			if (S[n2] == ':' && S[n1+1] == ' ')
			{
				S.remove(n1, 4);
				N -= 5;
				n2 -= 4;
			}
		}
		n2++;
		n1 = n2;
	}
}
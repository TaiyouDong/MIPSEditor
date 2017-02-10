#include"Preprocess.h"

void justify(QString& S)			//Ԥ����ɾ��ע�ͣ��հ׵���
{
	S = " lw $a0, 0($sp)\n addiu $a1, $sp, 4\n addiu $a2, $a1, 4\n sll $v0, $a0, 2\n addu $a2, $a2, $v0\n jal main\n nop\n li $v0, 10\n syscall\n" + S;
	S.replace('\t', " ");
	S.replace("    ", " ");
	S.replace("   ", " ");
	S.replace("  ", " ");
	int N = S.size();
	int i = 0;
	while (i + 1 < N)				//ÿ�п�ʼ�Ŀո񻻳�4���ո�
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
	while (!S.isEmpty() && (S[0] == ' ' || S[0] == '\n'))//ɾ����ͷ�Ŀհ׷�
		S.remove(0, 1);
	S.insert(0, "    ");
	//S.remove(S.length() - 1, 1);
}
void DeleteNode(QString&S)				//ɾ��ע��
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
void DeleteVoidLine(QString& S)			//ɾ������
{
	int n1 = 0, n2 = 0, i;
	int N = S.size();
	while (n2 < N && S[n2] != '\n')
		n2++;
	n1 = n2; n2++;
	while (n2 < N)
	{
		while (n2 < N && S[n2] != '\n')		//�ҵ���һ�����з�
			n2++;
		for (i = n1; i < n2; i++)
			if (S[i] != ' ' && S[i] != '\n') break;
		if (i == n2)						//n1 �� n2֮��ֻ�� ' ' �� '\n'����Ϊ���У�����Ҳ����ɾ���������ŵ�'\n'��
		{
			S.remove(n1, n2 - n1);
			N -= n2 - n1;					//����N��ֵ
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
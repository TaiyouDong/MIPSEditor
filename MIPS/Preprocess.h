#ifndef _PREPROCESS_H_
#define _PREPROCESS_H_
#include<QString>
#include<string>

void justify(QString& S);					//Ԥ����ɾ��ע�ͣ��հ׵���
void DeleteNode(QString&S);					//ɾ��ע��
void DeleteVoidLine(QString& S);			//ɾ������
bool Isspace(const std::string& str);
void StandLabel(QString&S);

#endif // !_PREPROCESS_H_

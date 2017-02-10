#ifndef _PREPROCESS_H_
#define _PREPROCESS_H_
#include<QString>
#include<string>

void justify(QString& S);					//Ô¤´¦Àí£¬É¾µô×¢ÊÍ£¬¿Õ°×µÄÐÐ
void DeleteNode(QString&S);					//É¾³ý×¢ÊÍ
void DeleteVoidLine(QString& S);			//É¾³ý¿ÕÐÐ
bool Isspace(const std::string& str);
void StandLabel(QString&S);

#endif // !_PREPROCESS_H_

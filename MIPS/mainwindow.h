#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QTextEdit>
#include<QPlaintextedit>
#include <QMenu>
#include <map>
#include <QString>
#include <set>
#include <vector>
#include "Assemble.h"
#include "InAssemble.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
	QString Translate16(const QString&);			//将16进制翻译成2进制
	QString Translate2(const QString&);
	void ShowRegister();
	int RegValue[32];
	void GetData(std::string &);

private:
	QLabel* msgLabel;
	QTextEdit* codeTrans;
	QTextEdit* AsmCode;
	QTextEdit* regValue;
	QTextEdit* BinaryCode;
	QTextEdit* textAddress;


	QMenu* fileMenu;
    QAction *openFile;
    QAction *saveFile;

	QMenu* buildMenu;
	QAction *Assemble;
	QAction *inAssemble;

	QMenu* dataMenu;
	QAction* openData;

	QMenu* styleMenu;
	QAction* HexStyle;
	QAction* BinaryStyle;

	QString HexCode;	//记录下来机器码框里的数据，便于进行进制转换
	QString BinCode;

	AssembleOpt AssOption;
	InAssembleOpt InAssOption;
	
	QTextEdit* Data;
	QString DataContain;
	

signals:

private slots:
    void OpenFile();
    void SaveFile();

	void OpenDataWindow();

	void AssembleCode();
	void InAssembleCode();

	void ChangeToHex();
	void ChangeToBinary();
	
};






#endif // MAINWINDOW_H

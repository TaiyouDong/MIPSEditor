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
	QString Translate16(const QString&);			//��16���Ʒ����2����
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

	QString HexCode;	//��¼�����������������ݣ����ڽ��н���ת��
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

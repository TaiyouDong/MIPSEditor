#include "mainwindow.h"
#include "Assemble.h"
#include "table.h"
#include "Preprocess.h"
#include "Convert.h"
#include <QtWidgets>
#include <QtGui>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QIODevice>
#include <QLabel>
#include <cctype>
#include <string>
#include <map>
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	memset(RegValue, 0, sizeof(int) * 32);
	openFile = new QAction(tr("&OpenFile"), this);			//
	openFile->setShortcut(tr("ctrl+O"));					//设置快捷键
	openFile->setStatusTip(tr("Open a file"));				//设置提示

	saveFile = new QAction(tr("&SaveFile"), this);
	saveFile->setShortcut(tr("ctrl+S"));
	saveFile->setStatusTip(tr("Save a file"));

	fileMenu = menuBar()->addMenu(tr("&File"));		//添加菜单栏
	fileMenu->addAction(openFile);
	fileMenu->addAction(saveFile);

	Assemble = new QAction(tr("&Assemble"), this);
	Assemble->setStatusTip(tr("Assemble the code"));
	Assemble->setShortcut(tr("ctrl+A"));
	inAssemble = new QAction(tr("&inAssemble"), this);
	inAssemble->setStatusTip(tr("Inassemble the code"));
	inAssemble->setShortcut(tr("alt+A"));

	buildMenu = menuBar()->addMenu(tr("&Bulid"));
	buildMenu->addAction(Assemble);
	buildMenu->addAction(inAssemble);

	openData = new QAction(tr("open&Data"), this);
	openData->setStatusTip(tr("Open the Data window"));
	openData->setShortcut(tr("ctrl+D"));

	dataMenu = menuBar()->addMenu(tr("&Data"));
	dataMenu->addAction(openData);

	HexStyle = new QAction(tr("&Hex"), this);
	HexStyle->setStatusTip(tr("Change to Hexadecimal"));
	HexStyle->setShortcut(tr("ctrl+H"));
	BinaryStyle = new QAction(tr("&Binary"), this);
	BinaryStyle->setStatusTip(tr("Change to Binary"));
	BinaryStyle->setShortcut(tr("ctrl+B"));

	styleMenu = menuBar()->addMenu(tr("&Style"));
	styleMenu->addAction(HexStyle);
	styleMenu->addAction(BinaryStyle);
	
	
    QToolBar *toolBar = addToolBar(tr("&File"));	//添加工具栏
    toolBar->addAction(openFile);
    toolBar->addAction(saveFile);
	toolBar->addAction(Assemble);
	toolBar->addAction(inAssemble);
	toolBar->addAction(HexStyle);
	toolBar->addAction(BinaryStyle);


    msgLabel = new QLabel;
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);

    statusBar()->addWidget(msgLabel);

    regValue = new QTextEdit(this);
    regValue->setReadOnly(true);
    regValue->setGeometry(10,90,190,710);
	QTextEdit* regValueTitle = new QTextEdit(this);
	regValueTitle->setReadOnly(true);
	regValueTitle->setGeometry(10, 50, 190, 35);
	regValueTitle->setFontPointSize(15);
	regValueTitle->setTextColor(Qt::red);
	regValueTitle->setText(tr("         Register"));

	BinaryCode = new QTextEdit(this);
	BinaryCode->setReadOnly(true);
	BinaryCode->setGeometry(210, 90, 360, 710);
	QTextEdit *BinaryCodeTitle = new QTextEdit(this);
	BinaryCodeTitle->setReadOnly(true);
	BinaryCodeTitle->setGeometry(210, 50, 360, 35);
	BinaryCodeTitle->setFontPointSize(15);
	BinaryCodeTitle->setTextColor(Qt::red);
	BinaryCodeTitle->setText(tr("                   Machine Code"));

	textAddress = new QTextEdit(this);
	textAddress->setReadOnly(true);
	textAddress->setGeometry(580, 90, 120, 710);
	QTextEdit *textAddressTitle = new QTextEdit(this);
	textAddressTitle->setReadOnly(true);
	textAddressTitle->setGeometry(580, 50, 120, 35);
	textAddressTitle->setFontPointSize(15);
	textAddressTitle->setTextColor(Qt::red);
	textAddressTitle->setText(tr(" BaseAddre"));

	AsmCode = new QTextEdit(this);
	AsmCode->setReadOnly(true);
	AsmCode->setGeometry(710, 90, 310, 710);
	AsmCode->setWordWrapMode(QTextOption::NoWrap);
	AsmCode->setCurrentFont(QFont());
	QTextEdit *AsmCodeTitle = new QTextEdit(this);
	AsmCodeTitle->setReadOnly(true);
	AsmCodeTitle->setGeometry(710, 50, 310, 35);
	AsmCodeTitle->setFontPointSize(15);
	AsmCodeTitle->setTextColor(Qt::red);
	AsmCodeTitle->setText(tr("            Asmemble Code"));

	codeTrans = new QTextEdit(this);								//画出文本框
	codeTrans->setReadOnly(true);
	codeTrans->setGeometry(1030, 90, 310, 710);
	//codeTrans->setFixedSize(codeTrans->width(), codeTrans->height());
	codeTrans->setWordWrapMode(QTextOption::NoWrap);				//设置水平滚动条
	QTextEdit* codeTransTitle = new QTextEdit(this);
	codeTransTitle->setReadOnly(true);
	codeTransTitle->setGeometry(1030,50,310,35);
	codeTransTitle->setFontPointSize(15);
	codeTransTitle->setTextColor(Qt::red);
	codeTransTitle->setText(tr("         Initial Asmemble Code"));

	ShowRegister();

	Data = new QTextEdit();
	Data->setWindowTitle("Data");

    connect(openFile, &QAction::triggered, this, &MainWindow::OpenFile);//信号槽，连接菜单和文件加载函数
    connect(saveFile, &QAction::triggered, this, &MainWindow::SaveFile);

	connect(Assemble, &QAction::triggered, this, &MainWindow::AssembleCode);
	connect(inAssemble, &QAction::triggered, this, &MainWindow::InAssembleCode);

	connect(HexStyle, &QAction::triggered, this, &MainWindow::ChangeToHex);
	connect(BinaryStyle, &QAction::triggered, this, &MainWindow::ChangeToBinary);

	connect(openData, &QAction::triggered, this, &MainWindow::OpenDataWindow);//打开Data窗口

}

MainWindow::~MainWindow()
{

}


void MainWindow::OpenFile()  
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),".",tr("Files(*.asm *.txt *.coe *.bin)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
		std::string path1 = path.toStdString();
		if (path1.find(".txt") != std::string::npos || path1.find(".asm") != std::string::npos)
		{
			codeTrans->setFontPointSize(14);				//设置字体大小
			QString s = in.readAll();
			justify(s);
			codeTrans->setFontFamily("Consolas");
			codeTrans->setText(s);
		}
		else if(path1.find(".coe") != std::string::npos )
		{
			BinaryCode->setFontPointSize(14);
			BinaryCode->setFontFamily("Consolas");
			QString str = in.readAll();
			str.remove('\n');
			str.remove(' ');
			str.remove('\t');
			str.replace(',', '\n');
			str = str.toUpper();
			std::string bstr = str.toStdString();
			int x = bstr.find('=');
			x++;
			int y = AssembleOpt::Findchar(bstr, x, ",; ");
			std::string tem = { bstr.begin() + x,bstr.begin() + y };
			int radix = StrToInt(tem);
			y = bstr.find('=',y);
			y++;
			bstr = bstr.substr(y);
			if (radix == 2)
			{
				BinCode = QString::fromStdString(bstr);
				BinCode.remove(';');
				BinaryCode->setText(BinCode);
				HexCode = Translate2(BinCode);
			}
			else
			{
				HexCode = QString::fromStdString(bstr);
				HexCode.remove(';');
				BinaryCode->setText(HexCode);
				BinCode = Translate16(HexCode);
			}
		}
		else if (path1.find(".bin") != std::string::npos)
		{
			std::ifstream fin(path1);
			BinaryCode->setFontPointSize(14);
			BinaryCode->setFontFamily("Consolas");
			char  c;
			int i = 0;
			std::string str((std::istreambuf_iterator<char>(fin)), (std::istreambuf_iterator<char>()));
			std::string binstr;
			while (i != str.size())
			{
				c = str[i];
				binstr.push_back(!!(c & 0x80) + '0');
				binstr.push_back(!!(c & 0x40) + '0');
				binstr.push_back(!!(c & 0x20) + '0');
				binstr.push_back(!!(c & 0x10) + '0');
				binstr.push_back(!!(c & 0x08) + '0');
				binstr.push_back(!!(c & 0x04) + '0');
				binstr.push_back(!!(c & 0x02) + '0');
				binstr.push_back(!!(c & 0x01) + '0');
				i++; 
				if (i % 4 == 0)
					binstr.push_back('\n');
			}
			binstr.pop_back();
			BinCode = QString::fromStdString(binstr);
			HexCode = Translate2(BinCode);
			BinaryCode->setText(HexCode);
			fin.close();
		}
        file.close();

    } /*else {
        QMessageBox::warning(this, tr("Path"),tr("You did not select any file."));
    }*/
}

void MainWindow::SaveFile()  
{
	QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", 
				tr("Text Files(*.txt);;COE Files(*.coe);;BIN Files(*.bin);;ASM Files(*.asm)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
		std::string path1 = path.toStdString();
		if (path1.find(".asm") != std::string::npos || path1.find(".txt") != std::string::npos)
		{
			out << AsmCode->toPlainText();
		}
		else if(path1.find(".bin") != std::string::npos) //输出二进制文件
		{
			std::ofstream fout(path.toStdString());
			std::string binstring;
			std::string str = BinCode.toStdString();
			std::string tem;
			int i = 0, j;
			std::string::size_type x;
			do
			{
				x = AssembleOpt::Findchar(str, i, '\n');
				tem = { str.begin() + i,str.begin() + x };
				for (j=0; j<32;)
				{
					char c = 0;
					for (int k = 0; k < 8; k++, j++)
					{
						c = (c << 1) + (tem[j] - '0');
					}
					fout << c;
				}
				i = x + 1;
			}while (x != str.size());
			fout.close();
		}
		else if (path1.find(".coe") != std::string::npos)
		{
			QString CoeStr = BinaryCode->toPlainText(), head;
			int bin;
			if (CoeStr.size() != 8)
			{
				if (CoeStr[8] == '\n')
					bin = 0;
				else bin = 1;
			}
			CoeStr.replace('\n',",\n");
			if (bin == 1)		//导出2进制模式
				head = "memory_initialization_radix = 2;\nmemory_initialization_vector =\n";
			else					//导出16进制模式
				head = "memory_initialization_radix = 16;\nmemory_initialization_vector =\n";
			CoeStr = head + CoeStr + ";";
			out << CoeStr;
		}
		file.close();
    }
}


void MainWindow::OpenDataWindow()
{
	Data->setFontPointSize(15);
	Data->setFontFamily("Consolas");
	Data->setText(DataContain);
	Data->setMinimumWidth(600);
	Data->setMinimumHeight(400);
	Data->setReadOnly(true);
	Data->show();
}

void MainWindow::AssembleCode()
{
	QString plaintext = codeTrans->toPlainText();
	if (!plaintext.isEmpty())
	{
		std::string str = plaintext.toStdString();
		AssOption.getLabelAddress(str,DataContain);
		AssOption.Replace(str);
		QString asmtext = QString::fromStdString(str);
		AsmCode->setFontPointSize(14);
		AsmCode->setFontFamily("Consolas");
		AsmCode->setText(asmtext);
		QString address;
		AssOption.AsmToBin(asmtext, HexCode, address);
		BinCode = Translate16(HexCode);
		textAddress->setFontPointSize(14);
		textAddress->setFontFamily("Consolas");
		textAddress->setText(address);
		BinaryCode->setFontPointSize(14);
		BinaryCode->setFontFamily("Consolas");
		BinaryCode->setText(HexCode);
	}
}
void MainWindow::InAssembleCode()
{
	QString plaintext = BinaryCode->toPlainText();
	if (!plaintext.isEmpty())
	{
		QString address, asmtext;
		InAssOption.BinToAsm(HexCode, asmtext, address);
		textAddress->setFontPointSize(14);
		textAddress->setFontFamily("Consolas");
		textAddress->setText(address);
		AsmCode->setFontPointSize(14);
		AsmCode->setFontFamily("Consolas");
		AsmCode->setText(asmtext);
		codeTrans->setText("");
	}
}

void MainWindow::ChangeToHex()
{
	BinaryCode->setFontPointSize(14);
	BinaryCode->setFontFamily("Consolas");
	BinaryCode->setText(HexCode);
}

void MainWindow::ChangeToBinary()
{
	BinaryCode->setFontPointSize(14);
	BinaryCode->setFontFamily("Consolas");
	BinaryCode->setText(BinCode);
}

QString MainWindow::Translate16(const QString& text)		//16进制的字符串翻译为2进制的字符串
{
	std::string str = text.toStdString();
	std::string tem, Bin;
	int i1 = 0, i2 = 0, N = text.size();
	do {
		i1 = i2;
		while (i2 < N && str[i2] != '\n')
			i2++;
		tem = { str.begin() + i1,str.begin() + i2 };
		if (tem.size() == 8)
		{
			for (int i = 0; i < tem.size(); i++)
			{
				Bin += RegDecoder::Decoder[tem[i]];
			}
			Bin += '\n';
		}
		i2++;
	} while (i2 < N);
	Bin.pop_back();
	return QString::fromStdString(Bin);
}
QString MainWindow::Translate2(const QString& text)		//2进制的字符串翻译为16进制的字符串
{
	std::string str = text.toStdString();
	std::string tem, Hex, s;
	int i1 = 0, i2 = 0, N = text.size();
	do {
		i1 = i2;
		while (i2 < N && str[i2] != '\n')
			i2++;
		tem = { str.begin() + i1,str.begin() + i2 };
		if (tem.size() == 32)
		{
			for (int i = 0; i < tem.size(); i+=4)
			{
				s = { tem.begin() + i,tem.begin() + i + 4 };
				Hex += RegDecoder::Decoder3[s];
			}
			Hex += '\n';
		}
		i2++;
	} while (i2 < N);
	Hex.pop_back();
	return QString::fromStdString(Hex);
}
void MainWindow::ShowRegister()
{
	std::string Register;
	for (int i = 0; i < 32; i++)
	{
		Register += IntToStr(i);
		if (i >= 10)
			Register += " ";
        else Register += "  ";
		Register += RegDecoder::REG_name[i];
		Register += " = ";
		Register += IntToStr(RegValue[i]);
		Register += "\n";
	}
	Register.pop_back();
    regValue->setFontFamily("Consolas");
    regValue->setFontPointSize(14);
    regValue->setText(QString::fromStdString(Register));
}










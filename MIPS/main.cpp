#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(300,150,1350,820);
    w.move ((QApplication::desktop()->width() - w.width())/2,
            (QApplication::desktop()->height() - w.height())/2);     //窗体居中显示
    w.setFixedSize(w.width(),w.height());   //固定窗体大小
    w.show();
    return a.exec();
}


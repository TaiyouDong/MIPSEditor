#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T18:53:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIPS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Assemble.cpp \
    Convert.cpp \
    InAssemble.cpp \
    PreProcess.cpp \
    table.cpp

HEADERS  += mainwindow.h \
    Assemble.h \
    InAssemble.h \
    Preprocess.h \
    table.h \
    Convert.h

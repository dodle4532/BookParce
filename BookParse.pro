#-------------------------------------------------
#
# Project created by QtCreator 2024-03-29T09:43:18
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookParse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    book.cpp \
    chapter.cpp \
    functions.cpp

HEADERS  += mainwindow.h \
    book.h \
    chapter.h \
    functions.h

FORMS    += mainwindow.ui

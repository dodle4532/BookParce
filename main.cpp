#include "mainwindow.h"
#include <QApplication>
#include <vector>
#include <locale>
#include <QDebug>
#include <codecvt>
#include <ctype.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Book Parce");
    w.show();

    return a.exec();
}

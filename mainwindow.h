#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "book.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_parseButton_clicked();

    void on_fileButton_clicked();

    void on_csvButton_clicked();

private:
    Ui::MainWindow *ui;
    Book book;
};

#endif // MAINWINDOW_H

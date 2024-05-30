#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <QDir>
#include <QString>
#include <QFileDialog>

enum class Pos;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->csvButton->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_parseButton_clicked()
{
    book.parse();
    ui->textField->setText(QString::fromStdString(book.getContent()));
    ui->csvButton->setVisible(true);
}

void MainWindow::on_fileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    QString::fromUtf8("Открыть файл"),
                                    QDir::currentPath(),
                                    "Text (*.txt))");
    Pos pagePos;
    if (ui->pagePosBox->currentText() == "Слева") {
        pagePos = Pos::LEFT;
    }
    else {
        pagePos = Pos::RIGHT;
    }
    book = Book(fileName.toStdString(), ui->whatToParce->currentText().toStdString(), pagePos);
}

void MainWindow::on_csvButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Выбрать папку"),
                                                        QDir::currentPath());
    book.saveToCsv(dirName.toStdString());
}

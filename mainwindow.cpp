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
    Pos pagePos;
    if (ui->pagePosBox->currentText() == "Слева") {
        pagePos = Pos::LEFT;
    }
    else {
        pagePos = Pos::RIGHT;
    }
    std::string indexWord = "";
    if (ui->indexComboBox->currentText() != "1.1") {
        std::string index = ui->indexComboBox->currentText().toStdString();
        index.pop_back();
        indexWord = index;
    }
    bool isNoSubchaptersIndex = true;
    if (ui->subchaptersComboBox->currentText() == "Разделены по индексам") {
        isNoSubchaptersIndex = false;
    }
    book = Book(path, ui->whatToParce->currentText().toStdString(), pagePos, indexWord, isNoSubchaptersIndex);
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
    path = fileName.toStdString();

}

void MainWindow::on_csvButton_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Выбрать папку"),
                                                        QDir::currentPath());
    book.saveToCsv(dirName.toStdString());
}

#include "book.h"
#include "functions.h"
#include <QDebug>
#include <cctype>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <iostream>

int fileLen;

Book::Book(std::string path) : path(path){}

void Book::parse() {
    std::fstream fd(path);
    int n = 0;
    parseNeed = false;
    int id = 1;
    std::string firstChapter; // Первая глава, как только мы ее встретим, то оглавление закончится
    while (!(fd.eof())) {
        n++;
        std::string line;
        std::getline(fd, line);
        removeSpacesFromBeginning(line);
        // Ищем начало оглавления
        if (line.find("Содержание") != std::string::npos) {
            parseNeed = true;
            continue;
        }
        // Завершаем чтение
        if (line.find(firstChapter) != std::string::npos && parseNeed == true && firstChapter != "") {
            break;
        }
        if (!parseNeed) {
            continue; // Если еще не дошли до оглавления, то продолжаем искать
        }
        auto pair = getChapterPageAndName(line);
        if (pair.first < 0) { // Некорректная страница - не то, что нам нужно
            continue;
        }
        if (getIdx(line) == "") { // Если нет индекса, то это глава, иначе это какая-то подглава
            chapters.emplace_back(pair.second, pair.first, id);
            if (firstChapter == "") {
                firstChapter = pair.second; // Устанавливаем первую главу
            }
        }
        else {
            chapters[chapters.size() - 1].addSubchapter(pair, id);
        }
        id++;
    }
    fileLen = n;
}

std::string Book::getContent() {
    std::string str;
    for (unsigned int i = 0; i < chapters.size(); ++i) {
        if (chapters[i].getStartPage() == -1) {
            continue;
        }
        str += chapters[i].getName() + "………………………………………………………………………………… " + std::to_string(chapters[i].getStartPage()) + "\n";
        for (int j = 0; j < chapters[i].getSubchaptersCount(); ++j) {
            str += "    " + chapters[i][j].getName() + "………………………………………………………………………………… " + std::to_string(chapters[i][j].getStartPage()) + "\n";
        }
    }
    return str;
}

int Book::getLastPage(int lastChapterStartPage) {
    std::vector<std::string> rows;
    std::fstream fd(path);
    while(!(fd.eof())) {
        std::string str;
        std::getline(fd, str);
        rows.push_back(str);
    }
    for (int i = rows.size() - 1; i > 0; --i) {
        int page = getPageNumber(rows[i]);
        if (page > lastChapterStartPage) {
            return page;
        }
    }
    return lastChapterStartPage + 50;
}

void Book::fillEndPages() {
    std::vector<Chapter*> elementsToLoop; //Элементы, по которым будем проходиться
    std::vector<Chapter*> tmp; // Временно будем сюда записывать то, по чему впоследствии будем проходиться
    for (int i = 0; i < chapters.size(); ++i) {
        elementsToLoop.push_back(&chapters[i]);
        if (i < chapters.size() - 1) {
            chapters[i].endPage = chapters[i + 1].startPage;
        }
        else {
            chapters[i].endPage = getLastPage(chapters[i].startPage);
        }
    }
    tmp = elementsToLoop;
    // Добавить сравнение, посмотреть, получится ли с ауто, добавить штуку на последнюю страницу
    while (tmp.size() != 0) {
        tmp.clear();
        for (auto& i: elementsToLoop) {
            for (int j = 0; j < i->subchapters.size(); ++j) {
                if (j < i->subchapters.size() - 1) {
                    i->subchapters[j].endPage = i->subchapters[j+1].startPage;
                }
                else {
                    i->subchapters[j].endPage = i->endPage;
                }
                if (i->subchapters[j].subchapters.size() > 0) { // Если у элемента есть подглавы, то по ним надо пройтись
                    tmp.push_back(&i->subchapters[j]);
                }
            }
        }
        elementsToLoop = tmp;
    }
}

void Book::saveToCsv(std::string path) {
    fillEndPages();
    std::ofstream fd(path + "/content.csv");
    fd << "id;index;title;start_page;end_page;parent_unit;level;\n";
    std::vector<std::pair<int, std::string>> result; // Вектор, по итогу состоящий из всех глав, хранящий номер Id и ряд CSV
    for (auto & i: chapters) {
        result.push_back({i.getId(), std::to_string(i.getId()) + ";" + i.getIndex() + ";" + i.getName()
                          + ";" + std::to_string(i.getStartPage()) + ";" + std::to_string(i.getEndPage()) + ";" + "1;"});
    }
    std::vector<Chapter*> elementsToLoop; //Элементы, по которым будем проходиться
    std::vector<Chapter*> tmp; // Временно будем сюда записывать то, по чему впоследствии будем проходиться
    for (auto &i : chapters) {
        elementsToLoop.push_back(&i);
    }
    tmp = elementsToLoop;
    int level = 2;
    while (tmp.size() != 0) {
        tmp.clear();
        for (auto& i: elementsToLoop) {
            for (auto& j: i->subchapters) {
                result.push_back({j.getId(), std::to_string(j.getId()) + ";" + j.getIndex() + ";" + j.getNameWOIndex()
                                 + ";" + std::to_string(j.getStartPage()) + ";" + std::to_string(j.getEndPage())
                                 + ";" + std::to_string(i->getId()) + ";" + std::to_string(level)});
                if (j.subchapters.size() > 0) { // Если у элемента есть подглавы, то по ним надо пройтись
                    tmp.push_back(&j);
                }
            }
        }
        elementsToLoop = tmp;
        level++;
    }
    // Сортируем результат по Id
    std::sort(result.begin(), result.end(), [](std::pair<int, std::string> p1, std::pair<int, std::string> p2) {
                                            return p1.first < p2.first;
    });
    for (auto &i : result) {
       fd << i.second << std::endl;
    }
}

#include "book.h"
#include "functions.h"
#include <QDebug>
#include <cctype>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <string>

int fileLen;

Book::Book(std::string path, std::string parseName, Pos pagePos, std::string indexWord, bool isNoSubchaptersIndex)
    : path(path), parseName(parseName), pagePos(pagePos), indexWord(indexWord), isNoSubchaptersIndex(isNoSubchaptersIndex){}



bool isFind(std::string line, std::string str) {
    QString s1 = QString::fromStdString(line).toLower();
    QString s2 = QString::fromStdString(str).toLower();
    if (s1.toStdString().find(s2.toStdString()) != std::string::npos) {
        return true;
    }
    return false;
}


void Book::parse() {
    std::fstream fd(path);
    int n = 0;
    parseNeed = false;
    bool isPrevWithIndex = false;
    bool isFirstWithIndex = false;
    int id = 1;
    std::string firstChapter; // Первая глава, как только мы ее встретим, то оглавление закончится
    while (!(fd.eof())) {
        n++;
        std::string line;
        std::getline(fd, line);
        removeSpacesFromBeginning(line);
        // Ищем начало оглавления
        if (n == 85) {
            std::cout << 1;
        }
        if (isFind(line, parseName)) {
            if (parseNeed == true) {
                continue;
            }
            parseNeed = true;
            continue;
        }
        // Завершаем чтение
        if (isFind(line, firstChapter) && parseNeed == true && firstChapter != "") {
            break;
        }
        if (!parseNeed || line == "") {
            continue; // Если еще не дошли до оглавления, то продолжаем искать
        }
        if (indexWord == "Глава ") {
            if (isFind(line, indexWord)) {
                line.erase(std::find(line.begin(), line.end(), indexWord[0]), std::find(line.begin(), line.end(), indexWord[0]) + indexWord.size());
            }
        }
        else if (isFind(indexWord, "Часть 1 Глава")) {
            removeSpacesFromEnding(indexWord);
            std::string firstWord;
            std::string secondWord;
            for (auto & i: indexWord) {
                if (i != ' ') {
                    firstWord += i;
                }
                else {
                    break;
                }
            }
            for (int i = indexWord.size() - 1; i >= 0; --i) {
                if (indexWord[i] != ' ') {
                    secondWord += indexWord[i];
                }
                else {
                    break;
                }
            }
            std::reverse(secondWord.begin(), secondWord.end());
            if (isFind(line, firstWord)) {
                line.erase(line.begin(), line.begin() + firstWord.size() + 1);
            }
            if (isFind(line, secondWord)) {
                line.erase(line.begin(), line.begin() + secondWord.size() + 1);
                std::string upperIdx = chapters[chapters.size() - 1].getIndex();
                line = upperIdx + line;
            }
        }
        auto pair = getChapterPageAndName(line, pagePos);
        if (pair.first < 0 && chapters.size() != 0) { // Некорректная страница - не то, что нам нужно
//            break;
            continue;
        }
        if (pair.second == "") {
            continue;
        }
        if (isPrevWithIndex) {
            if (isNoSubchaptersIndex && getIdx(line, pagePos) != "") {
                isPrevWithIndex = false;
            }
            std::string index = getIdx(line, pagePos);
            if (index != "") {
                if (!isSingleIndex(index)) {
                    chapters[chapters.size() - 1].addSubchapter(pair, id);
                    id++;
                    if (index != "") {
                        isPrevWithIndex = true;
                    }
                    continue;
                }
                else {
                    isPrevWithIndex = false;
                }
            }
        }
//        if (isPrevWithIndex && isNoSubchaptersIndex && getIdx(line, pagePos) != "") {
//            isPrevWithIndex = false;
//        }
        // Часть 1., Глава 1 - 1.
        if ((getIdx(pair.second, pagePos) == "" || chapters.size() == 0 || (((isSingleIndex(firstChapter)) || chapters.size() == 1) && isSingleIndex(pair.second)) || (isNoSubchaptersIndex && !isPrevWithIndex))
            && !(isPrevWithIndex && getIdx(pair.second, pagePos) == "" && isNoSubchaptersIndex)) { // Если нет индекса, то это глава, иначе это какая-то подглава
            chapters.emplace_back(pair.second, pair.first, id);
            if (isSingleIndex(getIdx(pair.second, pagePos))) {
                isPrevWithIndex = true;
            }
            if (firstChapter == "") {
                firstChapter = pair.second; // Устанавливаем первую главу
                if (getIdx(firstChapter, pagePos) != "") {
                    isFirstWithIndex = true;
                }
                removeSpacesFromBeginning(firstChapter);
                removeSpacesFromEnding(firstChapter);
            }
        }
        else {
            std::string index = chapters[chapters.size() - 1].getIndex();
            if (index != "" && isSingleIndex(index)) {
                try {
                    int lastI = std::stoi(index);
                    int curr = std::stoi(getIdx(pair.second, pagePos));
                    if (lastI + 1 == curr) {
                        chapters.emplace_back(pair.second, pair.first, id);
                        id++;
                        continue;
                    }
                }
                catch(...) {

                }
            }
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
        int page = getPageNumber(rows[i], pagePos);
        if (page > lastChapterStartPage && page < lastChapterStartPage + 50) {
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
        std::string name = i.getNameWOIndex();
        std::string index = i.getIndex();
        if (indexWord == "Глава " && index != "") {
            name = indexWord + index + " " + name;
            index = "";
        }
        else if (indexWord == "Часть 1 Глава" && index != "") {
            name = "Часть " + index + " " + name;
            index = "";
        }
        result.push_back({i.getId(), std::to_string(i.getId()) + ";" + index + ";" + name
                          + ";" + std::to_string(i.getStartPage()) + ";" + std::to_string(i.getEndPage()) + ";" + ";" + "1;"});
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
                std::string name = j.getNameWOIndex();
                std::string index = j.getIndex();
                if (indexWord == "Глава " && index != "") {
                    name = indexWord + index + " " + name;
                    index = "";
                }
                else if (indexWord == "Часть 1 Глава" && index != "") {
                    index.erase(index.begin(), index.begin() + getIndexBeginning(index).size());
                    name = "Глава " + index + " " + name;
                    index = "";
                }
                result.push_back({j.getId(), std::to_string(j.getId()) + ";" + index + ";" + name
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

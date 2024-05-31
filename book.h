#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <fstream>
#include <sstream>
#include "chapter.h"

// Класс, хранящий в себе всю информацию из оглавления
class Book
{
public:
    Book() = default;
    Book(std::string path, std::string parseName, Pos pagePos, std::string indexWord, bool isNoSubchaptersIndex);
    ~Book() = default;
    void parse();
    std::string getContent(); // Получаем оглавление в виде строки, чтобы вывести ее в textField
    void saveToCsv(std::string path);
    void fillEndPages();
    int getLastPage(int lastChapterStartPage);

private:
    std::vector<Chapter> chapters;
    Pos pagePos;
    std::string path;
    std::string parseName;
    std::string indexWord;
    bool isNoSubchaptersIndex;
    int currentPage = 0;
    bool parseNeed = true;
    bool isStartPassed = false;
};

#endif // BOOK_H

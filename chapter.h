#ifndef CHAPTER_H
#define CHAPTER_H

#include <vector>
#include <string>
#include <ostream>
enum class Pos{
    LEFT,
    RIGHT
};


// Класс, хранящий в себе информацию о главе(подглавы - главы, вложенную в эту главуб индекс - цифры перед названием)
class Chapter
{
public:
    Chapter(std::string name, int page, int id);
    Chapter(std::pair<int, std::string> p, int id);
    void setNewName(std::string str);
    std::string getName();
    std::string getNameWOIndex(); // Получить имя без индекса
    std::string getIndex();
    int getId();
    int getStartPage();
    int getEndPage();
    std::string getLastSubchapterName();
    int getSubchaptersCount();
    void addSubchapter(std::pair<int, std::string> nameAndPage, int id);

    Chapter& operator[] (int i);
    friend Chapter& getSubchapter(Chapter* ch, std::string idx);
    friend std::ostream& operator<< (std::ostream& out, Chapter& ch);

    friend class Book;
private:
    std::string name;
    std::string index; // Пример: Имя - '1.1. Введение, индекс - 1.1.
    int startPage;
    int endPage = -1;
    int id;
    std::vector<Chapter> subchapters;
};

#endif // CHAPTER_H

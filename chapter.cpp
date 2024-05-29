#include "chapter.h"
#include "functions.h"

Chapter::Chapter(std::string name, int startPage, int id) : name(name), startPage(startPage), id(id) {
    removeSpacesFromBeginning(this->name);
    index = getIdx(this->name);
}

Chapter::Chapter(std::pair<int, std::string> p, int id) : Chapter(p.second, p.first, id) {}

void Chapter::setNewName(std::string str) {
    name = str;
}

std::string Chapter::getName() {
    return name;
}

std::string Chapter::getNameWOIndex() {
    std::string copy = name;
    copy.erase(copy.begin(), copy.begin() + index.size());
    removeSpacesFromBeginning(copy);
    return copy;
}

std::string Chapter::getIndex() {
    return index;
}

int Chapter::getId() {
    return id;
}

int Chapter::getStartPage() {
    return startPage;
}

int Chapter::getEndPage() {
    return endPage;
}

std::string Chapter::getLastSubchapterName() {
    return subchapters[subchapters.size() - 1].getName();
}

int Chapter::getSubchaptersCount() {
    return subchapters.size();
}
// Основная проблема в том, чтобы понять, на какой уровень и куда конкретно надо добавить главу
void Chapter::addSubchapter(std::pair<int, std::string> pageAndName, int id) {
    if (subchapters.size() < 1) { // Если нет подглав, то мы точно добавляем подглаву на верхний уровень( иначе просто некуда)
        subchapters.emplace_back(pageAndName, id);
        return;
    }
    Chapter* element = this; // Элемент, который будет меняться по мере нашего углубления на уровни ниже
    std::string name = pageAndName.second;
    std::string idx = " ";
    while (idx != "") {
        if (isSingleIndex(name)) { // Если у нас остался единичный индекс, то мы на нужном уровне
            break;
        }
        idx = getIndexBeginning(getIdx(name));
        element = &getSubchapter(element, idx); // Меняем элемент, идем на уровень ниже
        name.erase(name.begin(), name.begin() + idx.size()); // Убираем один уровень из индекса имени
    }
//    if (element->subchapters.size() > 0) {
//        element->subchapters[element->subchapters.size() - 1].endPage = pageAndName.first;
//    }
    element->subchapters.emplace_back(pageAndName, id); // Добавляем куда надо
}

Chapter& Chapter::operator [](int i) {
    return subchapters[i];
}

Chapter& getSubchapter(Chapter* ch, std::string idx) {
    for (auto &i : ch->subchapters) {
        if (idx == i.getIndex()) {
            return i;
        }
    }
    return ch->subchapters[ch->subchapters.size() - 1];
}

std::ostream& operator<<(std::ostream& out, Chapter& ch) {
    out << ch.name << " " << ch.startPage << std::endl;
    for (auto & i : ch.subchapters) {
        out << i.getName() << " " << i.getStartPage() << std::endl;
    }
    return out;
}

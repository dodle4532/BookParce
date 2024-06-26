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
    removeNumbersFromBeginning(copy);
    removeSpacesFromBeginning(copy);
    removeSpacesFromEnding(copy);
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
    std::string indexBeg = getIndexBeginning(getIdx(name));
    if (indexBeg != index) {
        element = getSubchapter(element, indexBeg);
        if (element == nullptr) {
            subchapters.emplace_back(pageAndName, id);
            return;
        }
    }
    name.erase(name.begin(), name.begin() + getIndexBeginning(getIdx(name)).size());
    std::string idx = " ";
    if (getIdx(name) == "") {
        while (1) {
            if (element->subchapters.size() != 0 && element->subchapters[element->subchapters.size() - 1].getIndex() != "") {
                element = &element->subchapters[element->subchapters.size() - 1];
            }
            else {
                element->subchapters.emplace_back(pageAndName, id);
                return;
            }
        }
    }
    while (idx != "") {
        if (isSingleIndex(name)) { // Если у нас остался единичный индекс, то мы на нужном уровне
            break;
        }
        idx = getIndexBeginning(getIdx(name));
        element = getSubchapter(element, idx); // Меняем элемент, идем на уровень ниже
        name.erase(name.begin(), name.begin() + idx.size()); // Убираем один уровень из индекса имени
    }
    element->subchapters.emplace_back(pageAndName, id); // Добавляем куда надо
}

Chapter& Chapter::operator [](int i) {
    return subchapters[i];
}

Chapter* getSubchapter(Chapter* ch, std::string idx) {
    if (idx == "") {
        if (ch->getIndex() == "")
            return &ch->subchapters[ch->subchapters.size() - 1];
        else {
            return ch;
        }
    }
    for (auto &i : ch->subchapters) {
        std::string index = i.getIndex();
        if (!isSingleIndex(index)) {
            while (!(isSingleIndex(index))) {
                std::string indexBeg = getIndexBeginning(index);
                index.erase(index.begin(), index.begin() + indexBeg.size());
            }
        }
        if (idx == index) {
            return &i;
        }
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, Chapter& ch) {
    out << ch.name << " " << ch.startPage << std::endl;
    for (auto & i : ch.subchapters) {
        out << i.getName() << " " << i.getStartPage() << std::endl;
    }
    return out;
}

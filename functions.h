#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <algorithm>
#include "chapter.h"

int getPageNumber(std::string str);

std::pair<int, std::string> getChapterPageAndName(std::string str);

std::string getIndexBeginning(std::string index); // Получаем начало индекса(первое число до точки)

std::string getIdx(std::string str); // Получаем индекс из строки

bool isSingleIndex(std::string str); // Единичный ли это индекс или составной (1. - единичный, 1.1. - составной)

void removeSpacesFromBeginning(std::string& str);

#endif // FUNCTIONS_H

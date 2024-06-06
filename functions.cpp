#include "functions.h"

float getSimularityCoefficient(std::string str1, std::string str2) {
    str1.erase(std::remove_if(str1.begin(), str1.end(), [](unsigned char x) { return std::isspace(x); }), str1.end());
    str2.erase(std::remove_if(str2.begin(), str2.end(), [](unsigned char x) { return std::isspace(x); }), str2.end());
    if (str1 == str2) {
        return 1;
    }
    int commonCharacters = 0;
    int totalCharacters = std::max(str1.length(), str2.length());
    std::string sortedStr1 = str1;
    std::string sortedStr2 = str2;
    std::sort(sortedStr1.begin(), sortedStr1.end());
    std::sort(sortedStr2.begin(), sortedStr2.end());
    int i = 0;
    int j = 0;
    while (i < sortedStr1.length() && j < sortedStr2.length()) {
        if (sortedStr1[i] == sortedStr2[j]) {
            commonCharacters++;
            i++;
            j++;
        }
        else if (sortedStr1[i] < sortedStr2[j]) {
            i++;
        }
        else {
            j++;
        }
    }
    float similarityCoefficient = static_cast<float>(commonCharacters) / totalCharacters;
    return similarityCoefficient;
}

int getPageNumber(std::string str, Pos pagePos) {
//    if (str[0] != '\f') {
//        return {-1, chaptType::CHAPTER};
//    }
    if (str == "") {
        return -1;
    }
    removeSpacesFromBeginning(str);
    removeSpacesFromEnding(str);
    std::string number;
    //str.erase(str.begin());
    //chaptType tmp;
    if (pagePos == Pos::RIGHT) {
        if (std::isdigit(static_cast<unsigned char>(str[str.size() - 1])) != 0) {
          //  tmp = chaptType::SUBCHAPTER;
            //number.push_back(str[str.size() - 1]);
            for (int i = str.size() - 1; i >= 0; --i) {
                if (!(std::isdigit(static_cast<unsigned char>(str[i])))) {
                    break;
                }
                number.insert(number.begin(), str[i]);
            }
        }
    }
    else {
        if (std::isdigit(static_cast<unsigned char>(str[0])) != 0) {
            //number.push_back(str[0]);
            for (int i = 0; i < str.size(); ++i) {
                if (str[i] == ' ') {
                    break;
                }
                number.push_back(str[i]);
            }
        }
    }
    return number != "" ? std::stoi(number) : -1;
}

std::pair<int, std::string> getChapterPageAndName(std::string str, Pos pagePos) {
//    if (str[0] != '\f') {
//        return {-1, {"", chaptType::CHAPTER}};
//    }
    int p = getPageNumber(str, pagePos);
    std::string page = std::to_string(p);
    //str.erase(str.begin());
    if (page == "-1") {
        return {-1, ""};
    }
    std::string idx = getIdx(str);
    std::reverse(str.begin(), str.end());
    std::reverse(page.begin(), page.end());
    str.erase(str.find(page), str.find(page) + page.size());
    std::reverse(str.begin(), str.end());
    std::reverse(page.begin(), page.end());
    std::string substr;
//    for (auto & i : str) {
//        if (i == ' ') {
//            substr.push_back(i);
//        }
//        else {
//            str.erase(str.begin(), str.begin() + substr.size());
//            break;
//        }
//    }
    std::reverse(str.begin(), str.end());
    //substr.clear();
    for (auto & i : str) {
        if (i == ' ' || i == '.' || i == '…' || i == '_') {
            substr.push_back(i);
        }
        else {
            str.erase(str.begin(), str.begin() + substr.size());
            break;
        }
    }
    std::reverse(str.begin(), str.end());
    //5removeSpacesFromBeginning(str);
    return {p, str};
}

std::string getIndexBeginning(std::string index) {
    std::string result;
    for (auto & i : index) {
        if (std::isdigit(static_cast<unsigned char>(i)) != 0) {
            result.push_back(i);
        }
        else {
            break;
        }
    }
    if (result != "") {
        result.push_back('.');
    }
    return result;
}

std::string getIdx(std::string str, Pos pagePos) {
    std::string index;
    if (pagePos == Pos::LEFT) {
        removeNumbersFromBeginning(str);
        removeSpacesFromBeginning(str);
    }
    for (auto i : str) {
        if (std::isdigit(static_cast<unsigned char>(i)) != 0 || i == '.') {
                index.push_back(i);
        }
        else {
            break;
        }
    }
    return index;
}

bool isSingleIndex(std::string str) {
    std::string idx = getIdx(str);
    if (idx == "") {
        return false;
    }
    if (str[str.size() - 1] != '.') {
        str.push_back('.');
    }
    std::string index = getIndexBeginning(idx);
    str.erase(str.begin(), str.begin() + index.size());
    if (getIdx(str) == "") {
        return true;
    }
    return false;
}

void removeSpacesFromBeginning(std::string& str) {
    int spaceCount = 0;
    for (auto & i: str) {
        if (i == ' ' || i == '\t') {
            spaceCount++;
        }
        else {
            break;
        }
    }
    str.erase(str.begin(), str.begin() + spaceCount);
}

void removeSpacesFromEnding(std::string& str) {
    std::reverse(str.begin(), str.end());
    removeSpacesFromBeginning(str);
    std::reverse(str.begin(), str.end());
}

void removeNumbersFromBeginning(std::__cxx11::string &str) {
    int numberLen = 0;
    for (auto & i: str) {
        if (std::isdigit(static_cast<unsigned char>(i)) != 0) {
            numberLen++;
        }
        else {
            break;
        }
    }
    str.erase((str.begin()), str.begin() + numberLen);
}

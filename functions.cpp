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

int getPageNumber(std::string str) {
//    if (str[0] != '\f') {
//        return {-1, chaptType::CHAPTER};
//    }
    if (str == "") {
        return -1;
    }
    std::string number;
    //str.erase(str.begin());
    //chaptType tmp;
//    if (std::isdigit(static_cast<unsigned char>(str[0])) != 0) {
//        tmp = chaptType::CHAPTER;
//        number.push_back(str[0]);
//        for (int i = 1; i < str.size(); ++i) {
//            if (str[i] == ' ') {
//                break;
//            }
//            number.push_back(str[i]);
//        }
//    }
    if (std::isdigit(static_cast<unsigned char>(str[str.size() - 1])) != 0) {
      //  tmp = chaptType::SUBCHAPTER;
        number.push_back(str[str.size() - 1]);
        for (int i = str.size() - 2; i > 0; --i) {
            if (str[i] == ' ' || str[i] == '.') {
                break;
            }
            number.insert(number.begin(), str[i]);
        }
    }
    return number != "" ? std::stoi(number) : -1;
}

std::pair<int, std::string> getChapterPageAndName(std::string str) {
//    if (str[0] != '\f') {
//        return {-1, {"", chaptType::CHAPTER}};
//    }
    int p = getPageNumber(str);
    std::string page = std::to_string(p);
    //str.erase(str.begin());
    if (page == "-1") {
        return {-1, ""};
    }
    str.erase(str.find(page), str.find(page) + page.size());
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
        if (i == ' ' || i == '.' || i == '…') {
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
    result.push_back('.');
    return result;
}

std::string getIdx(std::string str) {
    std::string index;
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
    std::string index = getIndexBeginning(getIdx(str));
    str.erase(str.begin(), str.begin() + index.size());
    if (getIdx(str) == "") {
        return true;
    }
    return false;
}

void removeSpacesFromBeginning(std::string& str) {
    int spaceCount = 0;
    for (auto & i: str) {
        if (i == ' ') {
            spaceCount++;
        }
        else {
            break;
        }
    }
    str.erase(str.begin(), str.begin() + spaceCount);
}

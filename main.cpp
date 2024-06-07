#include "mainwindow.h"
#include <QApplication>
#include <vector>
#include <locale>
#include <QDebug>
#include <codecvt>
#include <ctype.h>

std::string ToLower(std::string string) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring ws = converter.from_bytes(string);
    std::wstring outWs;
    std::string outS;

    for (wchar_t wc : ws) {

       unsigned int c = (unsigned int)wc;
       switch (c) {
           case L'A': outWs += 'a'; break;
           case L'B': outWs += 'b'; break;
           case L'C': outWs += 'c'; break;
           case L'D': outWs += 'd'; break;
           case L'E': outWs += 'e'; break;
           case L'F': outWs += 'f'; break;
           case L'G': outWs += 'g'; break;
           case L'H': outWs += 'h'; break;
           case L'I': outWs += 'i'; break;
           case L'J': outWs += 'j'; break;
           case L'K': outWs += 'k'; break;
           case L'L': outWs += 'l'; break;
           case L'M': outWs += 'm'; break;
           case L'N': outWs += 'n'; break;
           case L'O': outWs += 'o'; break;
           case L'P': outWs += 'p'; break;
           case L'Q': outWs += 'q'; break;
           case L'R': outWs += 'r'; break;
           case L'S': outWs += 's'; break;
           case L'T': outWs += 't'; break;
           case L'U': outWs += 'u'; break;
           case L'V': outWs += 'v'; break;
           case L'W': outWs += 'w'; break;
           case L'X': outWs += 'x'; break;
           case L'Y': outWs += 'y'; break;
           case L'Z': outWs += 'z'; break;

           case L'А': outWs += L'а'; break;
           case L'Б': outWs += L'б'; break;
           case L'В': outWs += L'в'; break;
           case L'Г': outWs += L'г'; break;
           case L'Д': outWs += L'д'; break;
           case L'Е': outWs += L'е'; break;
           case L'Ж': outWs += L'ж'; break;
           case L'З': outWs += L'з'; break;
           case L'И': outWs += L'и'; break;
           case L'Й': outWs += L'й'; break;
           case L'К': outWs += L'к'; break;
           case L'Л': outWs += L'л'; break;
           case L'Н': outWs += L'н'; break;
           case L'М': outWs += L'м'; break;
           case L'О': outWs += L'о'; break;
           case L'П': outWs += L'п'; break;
           case L'Р': outWs += L'р'; break;
           case L'С': outWs += L'с'; break;
           case L'Т': outWs += L'т'; break;
           case L'У': outWs += L'у'; break;
           case L'Ф': outWs += L'ф'; break;
           case L'Х': outWs += L'х'; break;
           case L'Ц': outWs += L'ц'; break;
           case L'Ч': outWs += L'ч'; break;
           case L'Ш': outWs += L'ш'; break;
           case L'Щ': outWs += L'щ'; break;
           case L'Ь': outWs += L'ь'; break;
           case L'Ъ': outWs += L'ъ'; break;
           case L'Ы': outWs += L'ы'; break;
           case L'Э': outWs += L'э'; break;
           case L'Ю': outWs += L'ю'; break;
           case L'Я': outWs += L'я'; break;
           default: {
               outWs += wc; break;
           }
      }
}
    outS = converter.to_bytes(outWs);
    return outS;
}

//std::string toUpper(std::string str) {
//    for (auto & i : str) {
//        if (i >= 'a' && i <= 'z') {
//            i -=32;
//        }
//    }
//    std::vector<std::string> lowerKirillic = {"а", "б", "в", "г", "д", "е", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п",
//                            "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я" };
//    std::vector<std::string> upperKirillic = {"А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П",
//                            "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я" };
//    for (int i = 0; i < str.size(); ++i) {
//        std::string s = "" + str[i];
//        auto iter = std::find(lowerKirillic.begin(), lowerKirillic.end(), s);
//        if (iter != lowerKirillic.end()) {
//            int index = std::distance(lowerKirillic.begin(), iter);
//            str[i] = upperKirillic[index][0];
//        }
//    }
//    return str;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

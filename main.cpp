#include "mainwindow.h"
#include <QApplication>
#include <vector>

std::string toUpper(std::string str) {
    for (auto & i : str) {
        if (i >= 'a' && i <= 'z') {
            i -=32;
        }
    }
    std::vector<int> lowerKirillic = {'а', 'б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п',
                            'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я' };
    std::vector<int> upperKirillic = {'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П',
                            'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я' };
    for (int i = 0; i < str.size(); ++i) {
        auto iter = std::find(lowerKirillic.begin(), lowerKirillic.end(), static_cast<int>(str[i]));
        if (iter != lowerKirillic.end()) {
            int index = std::distance(lowerKirillic.begin(), iter);
            str[i] = upperKirillic[index];
        }
    }
    return str;
}

int main(int argc, char *argv[])
{
//    std::setlocale( LC_ALL,"Russian" );
    QApplication a(argc, argv);
//    std::string str = "Оглавлениеabc";
//    std::string e = toUpper(str);
//    setlocale(0, "");
    MainWindow w;
    w.show();

    return a.exec();
}

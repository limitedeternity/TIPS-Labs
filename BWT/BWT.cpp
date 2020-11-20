/*
 * Программа для осуществления преобразования Барроуза-Уиллера (двунаправленного)
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~1.5 часа.
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <algorithm> // для sort и distance
#include <vector>
#include <string>

using namespace std;

// Функция для осуществления циклического сдвига влево
// Н.п: ABACABA -> BACABAA
string lshift(string& s) {
    string s_copy = s;
    char c = s_copy[0];
    s_copy.erase(0, 1);
    return s_copy + c;
}

int main(void) {
    cout << "Input: ";
    string input;

    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> cyclicTable;
    cyclicTable.push_back(input);

    for (size_t i = 1; i < input.size(); i++) {
        cyclicTable.push_back(lshift(cyclicTable[i - 1]));
    }

    sort(cyclicTable.begin(), cyclicTable.end());
    vector<string>::iterator it = find(cyclicTable.begin(), cyclicTable.end(), input);
    int elemIndex = distance(cyclicTable.begin(), it);

    string lastColumn;
    for (size_t i = 0; i < cyclicTable.size(); i++) {
        lastColumn += cyclicTable[i][cyclicTable[i].size() - 1];
    }

    cout << "BWT(input) = (\"" << lastColumn << "\", " << elemIndex << ")" << endl;
    cout << "IBWT(\"" << lastColumn << "\", " << elemIndex << ") = ";

    string inverseBwt;
    vector<pair<char, int> > permutation;
    for (size_t i = 0; i < lastColumn.size(); i++) {
        permutation.push_back(pair<char, int>(lastColumn[i], i));
    }

    sort(permutation.begin(), permutation.end());
    for (size_t i = 0; i < lastColumn.size(); i++) {
        inverseBwt += permutation[elemIndex].first;
        elemIndex = permutation[elemIndex].second;
    }

    cout << inverseBwt << endl;
    return 0;
}

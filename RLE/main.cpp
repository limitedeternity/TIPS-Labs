#include <iostream>
#include <regex>
#include <string>

using namespace std;

/*
 * Демонстрация "сжатия" с использованием алгоритма RLE.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Начало: 5.10.2020 17:20:31
 * Финиш: 5.10.2020 19:15:53
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * */

bool str_replace(string& str, const string& from, const string& to) {
    int start_pos = str.find(from);
    if (start_pos == string::npos) return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// Перегрузка оператора * для достижения "умножения" строк: "ab" * 2 == "abab"
string operator * (string a, unsigned int b) {
    string output = "";
    while (b--) output += a;
    return output;
}

int main(void) {
    string input;
    cout << "Input: ";
    getline(cin, input);

    // Копии для проведения компрессии/декомпрессии
    string compress = input;
    string decompress = input;

    // "Ловит" повторения односимвольных строк, н.п: "ooooo"
    regex single_ch( "((\\w|\\d)\\2{1,})" );

    // "Ловит" повторения строк, состоящих из более, чем одного символа, н.п: "ababab"
    regex multi_ch( "((\\w+|\\d+)\\2{1,})" );

    // "Ловит" следующее выражение: [количество_повторений|строка]
    regex pattern( "(\\[(\\d+)\\|(\\w+|\\d+)\\])" );

    for (sregex_iterator it = sregex_iterator(compress.begin(), compress.end(), single_ch);
            it != sregex_iterator(); it++) {
        smatch match = *it;
        str_replace(compress, match.str(1), "[" + to_string(match.str(1).size() / match.str(2).size()) + "|" + match.str(2) + "]");
    }

    for (sregex_iterator it = sregex_iterator(compress.begin(), compress.end(), multi_ch);
            it != sregex_iterator(); it++) {
        smatch match = *it;
        str_replace(compress, match.str(1), "[" + to_string(match.str(1).size() / match.str(2).size()) + "|" + match.str(2) + "]");
    }

    cout << "Compressed: " << compress << endl;

    for (sregex_iterator it = sregex_iterator(decompress.begin(), decompress.end(), pattern);
            it != sregex_iterator(); it++) {
        smatch match = *it;
        str_replace(decompress, match.str(1), match.str(3) * stoi(match.str(2)));
    }

    cout << "Decompressed: " << decompress << endl;
    cout << "Test passed: " << (input.compare(decompress) == 0 ? "True" : "False") << endl;
    return 0;
}

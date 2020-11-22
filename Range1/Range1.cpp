/*
 * Программа для осуществления интервального кодирования (1 вариант).
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~30 минут
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cassert> // для assert

using namespace std;

template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

vector<unsigned> Range1(string text, string alphabet) {
    vector<unsigned> result;
    string concatAlphaText = alphabet + text;
    for (size_t i = alphabet.size(); i < concatAlphaText.size(); i++) {
        assert(alphabet.find(concatAlphaText[i]) != string::npos);

        size_t shiftBack = 0;
        while (concatAlphaText[i] != concatAlphaText[i - (shiftBack + 1)]) {
            shiftBack++;
        }

        result.push_back(shiftBack);
    }

    return result;
}

string IRange1(vector<unsigned> code, string alphabet) {
    string result = alphabet;
    for (size_t i = 0; i < code.size(); i++) {
        result.push_back(result[result.size() - 1 - code[i]]); 
    }

    return string(result.begin() + alphabet.size(), result.end());
}

string printableASCII() {
    return string("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \r\t\n");
}

int main(void) {
    string input;
    cout << "Input: ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<unsigned> code = Range1(input, printableASCII());
    cout << "Range1(\"" << input << "\") = [" << vecToString(code, ", ") << "]" << endl;
    cout << "IRange1([" << vecToString(code, ", ") << "]) = \"" << IRange1(code, printableASCII()) << "\"" << endl;
    return 0;
}

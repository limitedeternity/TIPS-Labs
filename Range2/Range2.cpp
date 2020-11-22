/*
 * Программа для осуществления интервального кодирования (2 вариант).
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~15 минут
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>
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

vector<unsigned> Range2(string text, string alphabet) {
    vector<unsigned> result;
    string concatAlphaText = alphabet + text;
    for (size_t i = alphabet.size(); i < concatAlphaText.size(); i++) {
        assert(alphabet.find(concatAlphaText[i]) != string::npos);

        set<char> uniqueVisitedChars;
        for (size_t j = 0; concatAlphaText[i] != concatAlphaText[i - (j + 1)]; j++) {
            uniqueVisitedChars.insert(concatAlphaText[i - (j + 1)]);
        }

        result.push_back(uniqueVisitedChars.size());
    }

    return result;
}

string IRange2(vector<unsigned> code, string alphabet) {
    string result = alphabet;
    for (size_t i = 0; i < code.size(); i++) {
        set<char> uniqueVisitedChars;
        size_t j = result.size() - 1;
        for (; uniqueVisitedChars.size() < code[i] || uniqueVisitedChars.count(result[j]); j--) {
            uniqueVisitedChars.insert(result[j]);
        }

        result += result[j];
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

    vector<unsigned> code = Range2(input, printableASCII());
    cout << "Range2(\"" << input << "\") = [" << vecToString(code, ", ") << "]" << endl;
    cout << "IRange2([" << vecToString(code, ", ") << "]) = \"" << IRange2(code, printableASCII()) << "\"" << endl;
    return 0;
}

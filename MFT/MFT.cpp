/*
 * Программа для осуществления трансформации Move-To-Front ("стопка книг").
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~30 минут.
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <algorithm> // для rotate
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

vector<unsigned> MFT(string text, string alphabet) {
    vector<unsigned> result;
    for (size_t i = 0; i < text.size(); i++) {
        string::iterator findChar = find(alphabet.begin(), alphabet.end(), text[i]);
        assert(findChar != alphabet.end());

        int charIndex = findChar - alphabet.begin();
        result.push_back(charIndex);
        rotate(alphabet.begin(), alphabet.begin() + charIndex, alphabet.begin() + charIndex + 1);
    }

    return result;
}

string IMFT(vector<unsigned> code, string alphabet) {
    string result;
    for (size_t i = 0; i < code.size(); i++) {
        assert(code[i] >= 0 && code[i] < alphabet.size());
        result += alphabet[code[i]];
        rotate(alphabet.begin(), alphabet.begin() + code[i], alphabet.begin() + code[i] + 1);
    }

    return result;
}

string printableASCII() {
    return string("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ \r\t\n");
}

int main(void) {
    string input;
    cout << "Input: ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<unsigned> code = MFT(input, printableASCII());
    cout << "MFT(\"" << input << "\") = [" << vecToString(code, ", ") << "]" << endl;
    cout << "IMFT([" << vecToString(code, ", ") << "]) = \"" << IMFT(code, printableASCII()) << "\"" << endl;
    return 0;
}

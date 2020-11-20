/*
 * Демонстрация перевода натуральных чисел в коды Левенштейна.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 2 + 3 = 5 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 * */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cmath> // для log2
#include <algorithm> // для transform

using namespace std;

vector<string> split(const string& s, const char delim = ' ') {
    vector<string> elems;
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}

// Перевод двоичного числа, представленного в виде вектора,
// в десятичное число.
unsigned binToDec(vector<unsigned> bin) {
    unsigned n = 0;
    for (size_t i = 0; i < bin.size(); i++) {
        n += bin[bin.size() - i - 1] * (1 << i);
    }

    return n;
}

// Перевод десятичного числа в двоичное, представленное в виде вектора.
vector<unsigned> decToBin(unsigned dec) {
    unsigned n = dec;
    vector<unsigned> result;
    while (n > 0) {
        result.insert(result.begin(), n % 2);
        n /= 2;
    }

    return result;
}

// Полиморфная конвертация вектора в строку
template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

// Кодирование
string levenEncode(unsigned n) {
    if (n == 0) return string(1, '0');

    vector<unsigned> code;
    vector<unsigned> binary;
    int nCopy = n;
    int counter = 0;
    while (binary.size() != 1) {
        binary = decToBin(nCopy);
        code.insert(code.begin(), binary.begin() + 1, binary.end());
        nCopy = log2(nCopy);
        counter++;
    }

    code.insert(code.begin(), 1, 0);
    code.insert(code.begin(), counter, 1);
    return vecToString(code);
}

// Декодирование
vector<unsigned> levenDecode(string code) {
    vector<unsigned> result;
    size_t i = 0;
    while (i < code.size()) {
        int offset = 0;
        while (code[i + offset++] == '1') {}
        if (offset - 1 == 0) {
            result.push_back(0);
            i += offset;
            continue;
        }

        unsigned n = 1;
        string encodedCmp = levenEncode(n);
        while (string(code.begin() + i, code.begin() + i + offset) != encodedCmp) {
            vector<char> slice(code.begin() + i + offset, code.begin() + i + offset + n);
            slice.insert(slice.begin(), '1');
            offset += n;

            vector<unsigned> bits;
            transform(slice.begin(), slice.end(), back_inserter(bits), [](char c) -> unsigned { return c - '0'; });
            n = binToDec(bits);
            encodedCmp = levenEncode(n);
        }

        result.push_back(n);
        i += encodedCmp.size();
    }

    return result;
}

int main(void) {
    string input;
    cout << "Positive numbers (space delimited): ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> strNumbers = split(input);
    vector<unsigned> numbers;
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(numbers), [](string s) -> unsigned { return stoi(s); });

    string levenStream;
    for (unsigned number : numbers) {
        levenStream += levenEncode(number);
    }

    cout << "Levenstein Code: " << levenStream << endl;
    cout << "From Levenstein: " << vecToString(levenDecode(levenStream), " ") << endl;
    return 0;
}

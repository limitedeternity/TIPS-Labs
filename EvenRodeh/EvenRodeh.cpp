/*
 * Программа для перевода чисел в коды Ивэн-Родэ и обратно.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 2 + 3 = 5 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <algorithm>

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

template<class T> vector<T> rjust(vector<T> vec, unsigned len, T justifier) {
    vector<T> vecCopy(vec);
    while (vecCopy.size() < len) {
        vecCopy.insert(vecCopy.begin(), justifier);
    }

    return vecCopy;
}

template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

unsigned binToDec(vector<unsigned> bin) {
    unsigned n = 0;
    for (size_t i = 0; i < bin.size(); i++) {
        n += bin[bin.size() - i - 1] * (1 << i);
    }

    return n;
}

vector<unsigned> decToBin(unsigned dec) {
    unsigned n = dec;
    vector<unsigned> result;
    while (n > 0) {
        result.insert(result.begin(), n % 2);
        n /= 2;
    }

    return result;
}

vector<unsigned> evenDecode(string code) {
    vector<unsigned> result;
    size_t i = 0;
    while (i < code.size()) {
        size_t shift = 3;
        
        vector<char> slice(code.begin() + i, code.begin() + i + shift);
        vector<unsigned> bits;
        transform(slice.begin(), slice.end(), back_inserter(bits), [](char c) -> unsigned { return c - '0'; });
        unsigned number = binToDec(bits);

        if (slice[0] == '0') {
            result.push_back(number);
            i += shift;
            continue;
        }

        while (code[i + shift] == '1') {
            slice.clear();
            bits.clear();
            slice = vector<char>(code.begin() + i + shift, code.begin() + i + shift + number);
            transform(slice.begin(), slice.end(), back_inserter(bits), [](char c) -> unsigned { return c - '0'; });
            number = binToDec(bits);
            shift += bits.size();
        }

        result.push_back(number);
        i += shift + 1;
    }

    return result;
}

string evenEncode(unsigned number) {
    vector<unsigned> code;
    if (number < 4) {
        code = decToBin(number);
        return vecToString(rjust(code, 3, (unsigned) 0));
    }

    code.push_back(0);
    while (number >= 8) {
        vector<unsigned> bin = decToBin(number);
        code.insert(code.begin(), bin.begin(), bin.end());
        number = bin.size();
    }

    vector<unsigned> bin = decToBin(number);
    code.insert(code.begin(), bin.begin(), bin.end());
    return vecToString(code);
}

int main(void) {
    string input;
    cout << "Positive numbers (space delimited): ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> strNumbers = split(input);
    vector<unsigned> numbers;
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(numbers), [](string s) -> unsigned { return stoi(s); });

    string evenStream;
    for (unsigned number : numbers) {
        evenStream += evenEncode(number);
    }

    cout << "Even-Rodeh Code: " << evenStream << endl;
    cout << "From Even-Rodeh: " << vecToString(evenDecode(evenStream), " ") << endl;
    return 0;
}

/*
 * Программа для кодирования положительных целых чисел
 * экспоненциальным кодом Голомба
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 3.5 часа
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cmath> // для log2, floor

#include "Unary.hpp"

using namespace std;

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

vector<unsigned> decToBin(unsigned dec) {
    unsigned n = dec;
    vector<unsigned> result;
    while (n > 0) {
        result.insert(result.begin(), n % 2);
        n /= 2;
    }

    return result;
}

unsigned binToDec(vector<unsigned> bin) {
    unsigned n = 0;
    for (size_t i = 0; i < bin.size(); i++) {
        n += bin[bin.size() - i - 1] * (1 << i);
    }

    return n;
}

int main(void) {
    unsigned k;
    cout << "k = ";
    if (cin.peek() == '\n') {
        k = 0;
        cout << "Assuming k = " << k << endl;
    } else if (!(cin >> k)) {
        cout << "Invalid k" << endl;
        return 1;
    }

    cin.ignore();
    vector<unsigned> numbers;
    cout << "Positive numbers (space delimited): ";
    while (cin.peek() != '\n') {
        unsigned n;
        if (!(cin >> n)) {
            cout << "Invalid n" << endl;
            return 1;
        }

        numbers.push_back(n);
    }

    cin.ignore();
    if (numbers.empty()) {
        cout << "No numbers to encode. Exiting..." << endl;
        return 0;
    }

    string expGolombStream;
    for (unsigned n : numbers) {
        double buff = (double) n / (double) (1 << k);
        unsigned w = 1 + floor(buff);
        unsigned f = floor(log2(1.0 + buff));

        string unaryF = Unary::encode(f);
        vector<unsigned> binaryW = rjust(decToBin(w), f, (unsigned) 0);
        string slicedBinaryW = vecToString(vector<unsigned>(binaryW.begin() + (binaryW.size() - f), binaryW.end()));
        vector<unsigned> binaryN = rjust(decToBin(n), k, (unsigned) 0);
        string slicedBinaryN = vecToString(vector<unsigned>(binaryN.begin() + (binaryN.size() - k), binaryN.end()));
        expGolombStream += unaryF + slicedBinaryW + slicedBinaryN;
    }

    cout << "Encoded: " << expGolombStream << endl;

    vector<unsigned> fromExpGolomb;
    size_t i = 0;
    while (i < expGolombStream.size()) {
        unsigned f = 0;
        size_t shift = 0;
        while (expGolombStream[i + shift++] == '1') {
            f++;
        }

        vector<char> RAsStr(expGolombStream.begin() + i + shift, expGolombStream.begin() + i + shift + f + k);
        vector<unsigned> RAsVec;
        transform(RAsStr.begin(), RAsStr.end(), back_inserter(RAsVec), [](char c) -> unsigned { return c - '0'; });
        unsigned r = binToDec(RAsVec);
        
        fromExpGolomb.push_back(((1 << f) - 1) * (1 << k) + r);
        i += shift + f + k;
    }

    cout << "Decoded: " << vecToString(fromExpGolomb, " ") << endl;
    return 0;
}

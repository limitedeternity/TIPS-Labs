/*
 * Программа для кодирования натуральных чисел старт-шаг-стоп (SSS) кодами.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 3.5 часа
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 */

#include <iostream>
#include <string>
#include <vector>
#include <cassert> // для assert
#include <functional> // для типа function
#include <numeric> // для accumulate
#include <algorithm> // для transform
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator

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

unsigned getUint(unsigned fallback, function<bool(unsigned)> condition) {
    unsigned uint;
    if (cin.peek() == '\n') {
        uint = fallback;
        cout << "No input. Assuming equal to " << fallback << endl; 
    } else {
        assert((cin >> uint) && "Invalid input");
        assert(condition(uint) && "Condition didn't meet");
    }

    cin.ignore();
    return uint;
}

vector<unsigned> getUintStream(function<bool(unsigned)> condition) {
    vector<unsigned> numbers;
    while (cin.peek() != '\n') {
        unsigned n;
        assert((cin >> n) && "Invalid input");
        assert(condition(n) && "Condition didn't meet");
        numbers.push_back(n);
    }

    cin.ignore();
    return numbers;
}

int main(void) {
    cout << "i = ";
    unsigned i = getUint(3, [](unsigned u) -> bool { return u > 0; }); 
    cout << "j = ";
    unsigned j = getUint(2, [](unsigned u) -> bool { return u > 0; });
    cout << "k = ";
    unsigned k = getUint(11, [](unsigned u) -> bool { return u > 0; });

    cout << "Natural numbers (space delimited): ";
    vector<unsigned> numbers = getUintStream([](unsigned u) -> bool { return u > 0; });
    if (numbers.empty()) {
        cout << "No numbers to encode. Exiting..." << endl;
        return 0;
    }

    unsigned k1 = (k - i) / j + 1;
    vector<unsigned> rangePowers;
    for (unsigned c = 0; c < k1; c++) {
        rangePowers.push_back(1 << (i + c * j));
    }

    vector<pair<unsigned, unsigned>> rangePairs;
    for (size_t c = 0; c < rangePowers.size(); c++) {
        rangePairs.push_back(
            pair<unsigned, unsigned>(
                accumulate(rangePowers.begin(), rangePowers.begin() + c, 0) + 1,
                accumulate(rangePowers.begin(), rangePowers.begin() + c + 1, 0)
            )
        );
    }

    string SSSStream;
    for (unsigned n : numbers) {
        for (size_t c = 0; c < rangePairs.size(); c++) {
            if (rangePairs[c].first <= n && n <= rangePairs[c].second) {
                SSSStream += Unary::encode(c) + vecToString(rjust(decToBin(n - rangePairs[c].first), i + c * j, (unsigned) 0));
                break;
            }
        }
    }

    cout << "Encoded: " << SSSStream << endl;

    vector<unsigned> fromSSS;
    size_t c = 0;
    while (c < SSSStream.size()) {
        unsigned rangeIndex = 0;
        size_t shift = 0;
        while (SSSStream[c + shift++] == '1') {
            rangeIndex++;
        }

        vector<char> rangeBitsAsStr(SSSStream.begin() + c + shift, SSSStream.begin() + c + shift + i + (rangeIndex * j));
        vector<unsigned> rangeBits;
        transform(rangeBitsAsStr.begin(), rangeBitsAsStr.end(), back_inserter(rangeBits), [](char c) -> unsigned { return c - '0'; });

        fromSSS.push_back(binToDec(rangeBits) + rangePairs[rangeIndex].first);
        c += shift + i + rangeIndex * j;
    }

    cout << "Decoded: " << vecToString(fromSSS, " ") << endl;
    return 0;
}

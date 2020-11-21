/*
 * Программа для кодирования положительных целых чисел кодом Голомба.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 4 часа
 * Компилятор: g++ (GCC) 10.2.0
 * Среда: Visual Studio 2019
 * Доп.флаги компиляции: -std=c++17
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <algorithm> // для transform
#include <cassert> // для assert

#include "Haffman.hpp"
#include "Unary.hpp"

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

unsigned log2(unsigned x) {
    unsigned z = x;
    unsigned result = 0;
    while (z >>= 1) result++;
    return result;
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
    unsigned m;
    cout << "m = ";
    if (cin.peek() == '\n') {
        m = 4;
        cout << "Assuming m = " << m << endl;
    } else if (!(cin >> m)) {
        cout << "Invalid m" << endl;
        return 1;
    } else {
        assert(m > 0 && "m shouldn't equal to 0");
        // Существует разновидность кода Голомба – код Райса,
        // иногда называемый "ленивым кодом Голомба".
        // Единственное его отличие от кода Голомба заключается в том,
        // что код Райса работает только тогда, когда m – степень 2:
        // -----------
        // assert((m & (m - 1)) == 0 && "m should be a power of 2");
    }

    cin.ignore();
    string remainderCodes;
    map<char, string> encodingTable;
    multimap<string, char> decodingTable;

    if (m & (m - 1)) {
        for (unsigned i = 0; i < m; i++) { 
            remainderCodes += (char) i; 
        }

        encodingTable = Haffman::buildEncodingTable(remainderCodes);
        decodingTable = Haffman::buildDecodingTable(encodingTable);
    }

    string input;
    cout << "Positive numbers (space delimited): ";
    while (cin.peek() != '\n') {
        unsigned n;
        if (!(cin >> n)) {
            cout << "Invalid n" << endl;
            return 1;
        }

        input += to_string(n) + ' ';
    }

    cin.ignore();
    if (input.empty()) {
        cout << "No numbers to encode. Exiting..." << endl;
        return 0;
    }

    input.pop_back();
    vector<string> strNumbers = split(input); 
    vector<unsigned> numbers;
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(numbers), [&](string s) -> unsigned { 
        unsigned n = stoi(s);
        assert(n % m < 256 && "Boundary condition didn't meet");
        return n; 
    });

    string golombStream;
    for (unsigned n : numbers) {
        if ((m & (m - 1)) == 0) {
            golombStream += Unary::encode(n / m) + vecToString(rjust(decToBin(n % m), log2(m), (unsigned) 0));
        } else {
            golombStream += Unary::encode(n / m) + encodingTable.find((char) n % m)->second;
        }
    }

    cout << "Encoded: " << golombStream << endl;

    vector<unsigned> fromGolomb;
    size_t i = 0;
    while (i < golombStream.size()) {
        unsigned gamma = 0;
        size_t shift = 0;
        while (golombStream[i + shift++] == '1') {
            gamma++;
        }

        unsigned delta;
        if ((m & (m - 1)) == 0) {
            vector<char> deltaAsStr(golombStream.begin() + i + shift, golombStream.begin() + i + shift + log2(m));
            vector<unsigned> deltaAsVec;
            transform(deltaAsStr.begin(), deltaAsStr.end(), back_inserter(deltaAsVec), [](char c) -> unsigned { return c - '0'; });
            delta = binToDec(deltaAsVec);
            i += shift + log2(m);
        } else {
            string selection;
            for (size_t cursor = 0; cursor < golombStream.size(); cursor++) {
                selection += golombStream[i + shift + cursor];
                map<string, char>::const_iterator search = decodingTable.find(selection);
                if (search != decodingTable.end()) {
                    delta = (unsigned) search->second;
                    break;
                }
            }

            i += shift + selection.size();
        }

        fromGolomb.push_back(gamma * m + delta);
    }

    cout << "Decoded: " << vecToString(fromGolomb, " ") << endl;
    return 0;
}

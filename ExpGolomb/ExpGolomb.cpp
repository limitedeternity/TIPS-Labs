/*
 * -std=c++11
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cmath> // для log2, floor

#include "Unary.hpp"

using namespace std;

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
        vector<unsigned> binaryW = decToBin(w);
        string slicedBinaryW = vecToString(vector<unsigned>(binaryW.begin() + (binaryW.size() - f), binaryW.end()));
        vector<unsigned> binaryN = decToBin(n);
        string slicedBinaryN = vecToString(vector<unsigned>(binaryN.begin() + (binaryN.size() - k), binaryN.end()));
        expGolombStream += unaryF + slicedBinaryW + slicedBinaryN;
    }

    cout << "Encoded: " << expGolombStream << endl;

    vector<unsigned> fromExpGolomb;
    size_t i = 0;
    /*while (i < expGolombStream.size()) {
        unsigned f = 0;
        size_t shift = 0;
        while (expGolombStream[i + shift++] == '1') {
            f++;
        }

        
    }*/

    return 0;
}

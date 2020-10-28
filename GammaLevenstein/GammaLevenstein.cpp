/*
 * Программа для перевода чисел в гамма-код Левенштейна и обратно.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~30 минут
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <algorithm> // для reverse, transform
#include <cassert> // для assert

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

unsigned binToDec(vector<unsigned>& bin) {
    unsigned n = 0;
    for (int i = bin.size() - 1; i >= 0; i--) {
        n += bin[i] * (1 << (bin.size() - i - 1));
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

template<class T> string vecToString(vector<T>& vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

int main(void) {
    string input;
    cout << "Natural numbers (space delimited): ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> strNumbers = split(input);
    vector<int> gammaLev;

    for (int i = 0; i < strNumbers.size(); i++) {
        int number = stoi(strNumbers[i]);
        assert(number > 0 && "Non-natural number detected");

        vector<unsigned> binaryNumber = decToBin(number);
        reverse(binaryNumber.begin(), binaryNumber.end());

        for (int j = 0; j < binaryNumber.size() - 1; j++) {
            gammaLev.push_back(0);
            gammaLev.push_back(binaryNumber[j]);
        }

        gammaLev.push_back(binaryNumber[binaryNumber.size() - 1]);
    }

    cout << "Encoded stream: " << vecToString(gammaLev) << endl; 
    cout << "Decoded numbers: ";
    vector<unsigned> decodedNumber;

    int i = 0, j = 1;
    while (i < gammaLev.size()) {
        if (gammaLev[i] == 1) {
            decodedNumber.push_back(1);
            reverse(decodedNumber.begin(), decodedNumber.end());
            cout << binToDec(decodedNumber) << " ";

            decodedNumber.clear();
            i++, j++;
            continue;
       }

       decodedNumber.push_back(gammaLev[j]);
       i += 2, j += 2;
    }

    cout << endl;
    return 0;
}

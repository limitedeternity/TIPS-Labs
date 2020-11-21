/*
 * Программа для перевода чисел в унарный код и обратно.
 * 
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~30 минут
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <algorithm> // для transform
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

template<class T> string vecToString(vector<T> vec, char const * delim = "") {
    ostringstream oss;
    if (!vec.empty()) {
        copy(vec.begin(), vec.end() - 1, ostream_iterator<T>(oss, delim));
        oss << vec.back();
    }

    return oss.str();
}

string unaryEncode(int n) {
    assert(n >= 0 && "Negative number detected");
    if (n == 0) return "0";
    else return "1" + unaryEncode(n - 1);
}

int main(void) {
    string input;
    cout << "Positive numbers (space delimited): ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> strNumbers = split(input); 
    vector<string> unaryEncoded;
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(unaryEncoded), [](string s) -> string { return unaryEncode(stoi(s)); });

    string bitstream = vecToString(unaryEncoded);
    cout << "Encoded stream: " << bitstream << endl;

    vector<string> splitBitstream = split(bitstream, '0');
    vector<unsigned> decodedNumbers;
    transform(splitBitstream.begin(), splitBitstream.end(), back_inserter(decodedNumbers), [](string s) -> unsigned { return s.size(); });
    
    cout << "Decoded numbers: " << vecToString(decodedNumbers, " ") << endl;
    return 0;
} 

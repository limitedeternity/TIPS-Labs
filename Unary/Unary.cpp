/*
 * ��������� ��� �������� ����� � ������� ��� � �������.
 * 
 * �����: �������� �. (3 ����, ��)
 * ����������� ����� ���������: ~30 �����
 * ����������: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // ��� ostringstream
#include <iterator> // ��� ostream_iterator
#include <algorithm> // ��� transform
#include <cassert> // ��� assert

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
    vector<int> numbers; 
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(numbers), [](string s) -> int { return stoi(s); });

    vector<string> unaryEncoded;
    transform(numbers.begin(), numbers.end(), back_inserter(unaryEncoded), unaryEncode);

    string bitstream = vecToString(unaryEncoded);
    cout << "Encoded stream: " << bitstream << endl;

    vector<string> splitBitstream = split(bitstream, '0');
    vector<int> decodedNumbers;
    transform(splitBitstream.begin(), splitBitstream.end(), back_inserter(decodedNumbers), [](string s) -> int { return s.size(); });
    
    cout << "Decoded numbers: " << vecToString(decodedNumbers, " ") << endl;
    return 0;
} 

/*
 * Демонстрация перевода натуральных чисел в коды Левенштейна.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Начало: 22.10.2020 19:15
 * Финиш: 22.10.2020 20:47
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * */

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cmath> // для log2

using namespace std;

// Перевод двоичного числа, представленного в виде вектора,
// в десятичное число.
unsigned binToDec(vector<unsigned> bin) {
    unsigned n = 0;
    for (int i = bin.size() - 1; i >= 0; i--) {
        n += bin[i] * (1 << (bin.size() - i - 1));
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
vector<unsigned> levenEncode(unsigned n) {
    if (n == 0) return vector<unsigned>(1, 0);

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
    return code;
}

// Декодирование
unsigned levenDecode(vector<unsigned> code) {
    if (code == vector<unsigned>(1, 0)) return 0;

    int ones = 0;
    for (int i = 0; i < code.size(); i++) {
        if (code[i] == 0) break;
        ones++;
    }

    int n = 1;
    int offset = ones + 1;
    for (int i = ones - 1; i > 0; i--) {
        if (offset < code.size() && offset + n < code.size()) {
            binary = vector<unsigned>(code.begin() + offset, code.begin() + offset + n);
        }
        else if (offset < code.size() && offset + n >= code.size()) {
            binary = vector<unsigned>(code.begin() + offset, code.end());
        }

        binary.insert(binary.begin(), 1, 1);
        offset += n;
        n = binToDec(binary);
    }

    return n;
}

int main(void) {
    for (int i = 0; i < 16; i++) {
        vector<unsigned> encoded = levenEncode(i);

        cout << "Number: " << i << endl;
        cout << "Encoded: " << vecToString(encoded) << endl;
        cout << "Decoded: " << levenDecode(encoded) << endl << endl;
    }

    return 0;
}

/*
 * Программа для демонстрации метода Шеннона
 * для конструирования двоичных префиксных кодов.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~5 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <algorithm> // для reverse
#include <vector>
#include <string>
#include <sstream> // для ostringstream
#include <iterator> // для ostream_iterator
#include <cassert> // для assert

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

// Полиморфное выравнивание вектора справа
template<class T> vector<T> rjust(vector<T> vec, unsigned len, T justifier) {
    vector<T> vecCopy(vec);
    while (vecCopy.size() < len) {
        vecCopy.insert(vecCopy.begin(), justifier);
    }

    return vecCopy;
}

// Полиморфное выравнивание вектора слева
template<class T> vector<T> ljust(vector<T> vec, unsigned len, T justifier) {
    vector<T> vecCopy(vec);
    while (vecCopy.size() < len) {
        vecCopy.push_back(justifier);
    }

    return vecCopy;
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

int main(void) {
    unsigned numberOfCodes;
    cout << "Number of codes: ";
    cin >> numberOfCodes;

    if (numberOfCodes == 0) return 0;

    unsigned len;
    vector<unsigned> codeLengths;
    double totalSum = 0.0;

    for (int i = 0; i < numberOfCodes; i++) {
        cout << "(" << i + 1 << ") Code length: ";
        cin >> len;
        codeLengths.push_back(len);
        totalSum += 1.0 / (double)(1 << len);
    }

    for (int i = 0; i < numberOfCodes - 1; i++) {
        assert(codeLengths[i] <= codeLengths[i + 1] && "Code length condition didn't meet");
    }

    // Проверяем неравенство Крафта
    assert(totalSum < 1.0 && "Kraft's inequality condition didn't meet");

    vector<unsigned> curr(codeLengths[0]);
    fill(curr.begin(), curr.end(), 0);
    cout << vecToString(curr) << endl;

    for (int i = 0; i < numberOfCodes - 1; i++) {
        vector<unsigned> assignee = decToBin(1 << codeLengths[i]);
        assignee = vector<unsigned>(assignee.begin(), assignee.begin() + codeLengths[i]);
        reverse(assignee.begin(), assignee.end());

        vector<unsigned> sum = decToBin(binToDec(curr) + binToDec(assignee));
        sum = rjust(sum, codeLengths[i], (unsigned) 0);
        sum = ljust(sum, codeLengths[i + 1], (unsigned) 0);
        curr = sum;

        cout << vecToString(curr) << endl;
    }

    return 0;
}

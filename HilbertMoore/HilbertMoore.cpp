/*
 * Программа для кодирования методом Гилберта-Мура.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: ~1.5 часа
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator> // для const_iterator
#include <numeric> // для accumulate
#include <cmath> // для ceil, log2
#include <cassert> // для assert

using namespace std;

// Функция для перевода дробных десятичных чисел меньше единицы и больше нуля
// в двоичную систему счисления
string fracDecimalToBin(double num, int precis) {
    assert(num < 1.0 && num > 0.0 && "Number should be <1 and >0");
    string binary;

    while (precis--) {
        num *= 2;
        int frac_bit = num;
        num -= frac_bit;
        binary.push_back(frac_bit + '0');
    }

    return binary;
}

int main(void) {
    cout << "Input: ";

    string input;
    getline(cin, input);
    if (input.empty()) return 0;

    // Частотный анализ
    map<char, int> freqTable;
    for (size_t i = 0; i < input.size(); i++) {
        freqTable[input[i]] += 1;
    }

    // Делим map на два вектора (ключи и значения, символы и вероятности соотв.)
    vector<char> chars;
    vector<double> probabilities;
    for (map<char, int>::const_iterator it = freqTable.begin(); it != freqTable.end(); it++) {
        chars.push_back(it->first);
        probabilities.push_back((double)it->second / (double)input.size());
    }

    // Шаг 1: расчёт кумулятивных вероятностей
    vector<double> cumulative;
    for (size_t i = 0; i < probabilities.size(); i++) {
        vector<double> slice = vector<double>(probabilities.begin(), probabilities.begin() + i);
        cumulative.push_back(accumulate(slice.begin(), slice.end(), 0.0) + probabilities[i] / 2.0);
    }

    // Шаг 2: расчёт длин результирующих кодов
    vector<int> lengths;
    for (size_t i = 0; i < probabilities.size(); i++) {
        lengths.push_back(ceil(-log2(probabilities[i])) + 1);
    }

    // Шаг 3: построение кодов и их вывод
    vector<string> codes;
    for (size_t i = 0; i < cumulative.size(); i++) {
        codes.push_back(fracDecimalToBin(cumulative[i], lengths[i]));
        cout << chars[i] << " -> " << codes[i] << endl;
    }

    return 0;
}

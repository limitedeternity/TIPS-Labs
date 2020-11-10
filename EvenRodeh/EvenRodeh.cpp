/*
 * Демонстрация перевода чисел в коды Ивэн-Родэ и обратно.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 2 часа
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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

unsigned binToDec(vector<unsigned> bin) {
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

int evenDecode(vector<unsigned> code) {
    size_t i = 3;
    vector<unsigned> bits = vector<unsigned>(code.begin(), code.begin() + i);
    int number = binToDec(bits);
    if (bits[0] == 0) return number;

    do {
        if (code[i] == 0) return number;
        bits = vector<unsigned>(code.begin() + i, code.begin() + i + number);
        number = binToDec(bits);
        i += bits.size();
    } while (i < code.size());
    return number;
}

vector<unsigned> evenEncode(int number) {
    vector<unsigned> code;
    if (number < 4) {
        code = decToBin(number);
        return rjust(code, 3, (unsigned) 0);
    }

    code.push_back(0);
    while (number >= 8) {
        vector<unsigned> bin = decToBin(number);
        code.insert(code.begin(), bin.begin(), bin.end());
        number = bin.size();
    }

    vector<unsigned> bin = decToBin(number);
    code.insert(code.begin(), bin.begin(), bin.end());
    return code;
}

int main(void) {
    for (int i = 0; i < 18; i++) {
        vector<unsigned> encode = evenEncode(i);
        cout << "Number: " << i << "; Encode: " << vecToString(encode) << "; Decode: " << evenDecode(encode) << endl;
    }

    return 0;
}

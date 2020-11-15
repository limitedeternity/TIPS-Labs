#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm> // для transform, move

using namespace std;

/*
 * Программа для перевода чисел в коды Элиаса – 
 * универсальные коды для кодирования натуральных чисел.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 12 + 4 = 16 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 * Используемая литература:
 * - Ватолин Д. и Ко: "Методы сжатия данных. Устройство архиваторов, сжатие изображений и видео."
 * */

// Функция, вычисляющая <целочисленный> логарифм по основанию 2.
unsigned log2(unsigned x) {
    unsigned z = x;
    unsigned result = 0;
    while (z >>= 1) result++;
    return result;
}

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

unsigned binToDec(vector<unsigned>& bin) {
    unsigned n = 0;
    for (int i = bin.size() - 1; i >= 0; i--) {
        n += bin[i] * (1 << (bin.size() - i - 1));
    }

    return n;
}

/* 
 * Алгоритм построения гамма-кода: 
 *   - Записать число в двоичном представлении
 *   - Перед двоичным представлением дописать нули,
 *   количество которых на единицу меньше количества битов
 *   двоичного представления.
 *
 * Например: 1111 -> 0001111.
 * */
string eliasGamma(unsigned bin) {
    string result;

    // Количество занимаемых числом битов
    unsigned bits = log2(bin) + 1;

    // > "дописать нули, количество которых на единицу меньше количества битов"
    // Сдвиг вправо на количество бит большее, чем занимает число, всегда возвращает 0.
    for (unsigned i = 0; i < bits * 2 - 1; i++) {
        // Получает младший бит и вставляет его в начало
        // Например: 
        // 1) 1101 >> 1 = 110
        // 2) 110 & 1 = 0
        result.insert(result.begin(), ((bin >> i) & 1) + '0');
    }

    return result;
}

/*
 * Алгоритм декодирования гамма-кода:
 *   - Сдвигаем единицу на количество нулей влево 
 *   - Пропускаем первую единицу, прибавляем к остатку результат сдвига
 *
 * Например: 
 *   - 0001111 -> 0001 111
 *   - 1 << 3 = 1000
 *   - 1000 + 111 = 1111
 * */

vector<unsigned> gammaDecode(string gamma) {
    vector<unsigned> result;
    size_t i = 0;
    while (i < gamma.size()) {
        int zeros = 0;
        size_t shift = 0;
        while (gamma[i + shift++] == '0') {
            zeros++;
        }

        if (zeros == 0) {
            result.push_back(1); i++;
            continue;
        }

        vector<char> numberAsStr(gamma.begin() + i + shift, gamma.begin() + i + shift + zeros);
        vector<unsigned> number;
        transform(numberAsStr.begin(), numberAsStr.end(), back_inserter(number), [](char c) -> unsigned { return c - '0'; });
        result.push_back(binToDec(number) + (1 << zeros));
        i += shift + zeros;
    }

    return result;
}

/*
 * Алгоритм построения дельта-кода:
 * - Закодировать количество значащих битов числа с помощью гамма-кода.
 * - Дописать к закодированному количеству значащих битов справа само число
 *   без старшей единицы.
 * */
string eliasDelta(unsigned bin) {
    unsigned bits = log2(bin) + 1;
    string gammaBits = eliasGamma(bits);
    string result;

    for (int i = 0; i < bits - 1; i++) {
        result.insert(result.begin(), ((bin >> i) & 1) + '0');
    }

    // Я сделал наоборот: дописал закодированное количество бит слева
    // от числа без старшей единицы
    result.insert(result.begin(), gammaBits.begin(), gammaBits.end());
    return result;
}

/*
 * Алгоритм декодирования дельта-кода:
 * - Считаем количество нулей до первой единицы (М)
 * - Читаем следующие М бит. Получаем часть исходного числа (L).
 * - Читаем следующие L - 1 бит (T).
 * - Убираем у L справа M бит, присоединяем T, восстанавливаем число.
 * */
vector<unsigned> deltaDecode(string delta) {
    vector<unsigned> result;
    size_t i = 0;
    while (i < delta.size()) {
        int zeros = 0;
        size_t shift = 0;
        while (delta[i + shift++] == '0') {
            zeros++;
        }

        if (zeros == 0) {
            result.push_back(1); i++;
            continue;
        }

        vector<char> LAsStr(delta.begin() + i + zeros, delta.begin() + i + zeros * 2 + 1);
        vector<unsigned> LAsVec;
        transform(LAsStr.begin(), LAsStr.end(), back_inserter(LAsVec), [](char c) -> unsigned { return c - '0'; });
        unsigned L = binToDec(LAsVec);

        vector<char> TAsStr(delta.begin() + i + zeros * 2 + 1, delta.begin() + i + zeros * 2 + 1 + L - 1);
        vector<unsigned> TAsVec;
        transform(TAsStr.begin(), TAsStr.end(), back_inserter(TAsVec), [](char c) -> unsigned { return c - '0'; });

        vector<unsigned> number;
        move(LAsVec.begin(), LAsVec.end() - zeros, back_inserter(number));
        move(TAsVec.begin(), TAsVec.end(), back_inserter(number));
        
        result.push_back(binToDec(number));
        i += zeros * 2 + 1 + L - 1;
    }

    return result;
}

/*
 * Алгоритм построения омега-кода:
 * - В конец представления записать 0.
 * - Если число != 1, слева добавить двоичное представление числа
 * - В N записать новое значение – количество только что записанных бит
 *   минус 1.
 * - Вернуться к шагу 2.
 * */

string eliasOmega(unsigned bin) {
    string result;
    result.push_back('0');
    
    unsigned n = bin;
    while (n != 1) {
        unsigned bits = log2(n) + 1;
        for (unsigned i = 0; i < bits; i++) {
            result.insert(result.begin(), ((n >> i) & 1) + '0');
        }

        n = bits - 1;
    }

    return result;
}

/*
 * Алгоритм декодирования омега-кода:
 * - Записываем в N единицу
 * - Считываем старший бит. Если он равен 1, то считываем группу бит
 *   длиной N+1 в N. В противном случае, N – восстановленное число.
 * - Удаляем считанную группу и продолжаем.
 * */

vector<unsigned> omegaDecode(string omega) {
    vector<unsigned> result;
    size_t i = 0;
    while (i < omega.size()) {
        unsigned n = 1;
        while (omega[i] == '1') {
            vector<char> slice(omega.begin() + i, omega.begin() + i + n + 1);
            vector<unsigned> newN;
            transform(slice.begin(), slice.end(), back_inserter(newN), [](char c) -> unsigned { return c - '0'; });
            unsigned tempN = binToDec(newN);

            i += n + 1;
            if (tempN != 0) {
                n = tempN;
            }
        } 

        result.push_back(n);
        i++;
    }

    return result;
}

int main(void) {
    string input;
    cout << "Natural numbers (space delimited): ";
    getline(cin, input);
    if (input.empty()) return 0;

    vector<string> strNumbers = split(input);
    vector<unsigned> numbers;
    transform(strNumbers.begin(), strNumbers.end(), back_inserter(numbers), [](string s) -> unsigned { return stoi(s); });
    
    string eliasGammaStream;
    for (unsigned number : numbers) {
        eliasGammaStream += eliasGamma(number);
    }

    cout << "Elias Gamma: " << eliasGammaStream << endl;
    cout << "From Gamma: " << vecToString(gammaDecode(eliasGammaStream), " ") << endl;

    string eliasDeltaStream;
    for (unsigned number : numbers) {
        eliasDeltaStream += eliasDelta(number);
    }

    cout << "Elias Delta: " << eliasDeltaStream << endl;
    cout << "From Delta: " << vecToString(deltaDecode(eliasDeltaStream), " ") << endl;

    string eliasOmegaStream;
    for (unsigned number : numbers) {
        eliasOmegaStream += eliasOmega(number);
    }

    cout << "Elias Omega: " << eliasOmegaStream << endl;
    cout << "From Omega: " << vecToString(omegaDecode(eliasOmegaStream), " ") << endl;
    return 0;
}

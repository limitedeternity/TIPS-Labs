#include <iostream>
#include <vector> 
#include <sstream>
#include <iterator>

using namespace std;

/*
 * Демонстрация перевода чисел в коды Элиаса – универсальные коды для кодирования 
 * натуральных чисел.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Начало: 26.09.2020 13:10:57
 * Финиш: 27.09.2020 1:00:20
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Используемая литература:
 * - Ватолин Д. и Ко: "Методы сжатия данных. Устройство архиваторов, сжатие изображений и видео."
 * */

/* 
 * Алгоритм построения гамма-кода: 
 *   - Записать число в двоичном представлении
 *   - Перед двоичным представлением дописать нули,
 *   количество которых на единицу меньше количества битов
 *   двоичного представления.
 *
 * Например: 1111 -> 0001111.
 * */

// Функция, вычисляющая <целочисленный> логарифм по основанию 2.
unsigned log2(unsigned x) {
    unsigned z = x;
    unsigned result = 0;
    while (z >>= 1) result++;
    return result;
}

vector<unsigned> eliasGamma(unsigned bin) {
    // Используется вектор, потому что bitset фиксированного размера.
    vector<unsigned> result;

    // Количество занимаемых числом битов
    unsigned bits = log2(bin) + 1;

    // > "дописать нули, количество которых на единицу меньше количества битов"
    // Сдвиг вправо на количество бит большее, чем занимает число, всегда возвращает 0.
    for (int i = 0; i < bits * 2 - 1; i++) {
        // Получает младший бит и вставляет его в начало вектора
        // Например: 
        // 1) 1101 >> 1 = 110
        // 2) 110 & 1 = 0
        result.insert(result.begin(), (bin >> i) & 1);
    }

    return result;
}

/*
 * Алгоритм декодирования гамма-кода:
 *   - Сдвигаем единицу на количество нулей влево 
 *   - Пропускаем первую единицу, прибавляем к остатку результат сдвига
 *
 * Например: 
 *   - 0001111 -> 000 111
 *   - 1 << 3 = 1000
 *   - 1000 + 111 = 1111
 * */

// Я просто обрежу вектор по количеству нулей:
// - [0,0,0,1,1,1,1].size() / 2 = 3
vector<unsigned> gammaDecode(vector<unsigned> gamma) {
    return vector<unsigned>(gamma.begin() + gamma.size() / 2, gamma.end());
}

/*
 * Алгоритм построения дельта-кода:
 * - Закодировать количество значащих битов числа с помощью гамма-кода.
 * - Дописать к закодированному количеству значащих битов справа само число
 *   без старшей единицы.
 * */
vector<unsigned> eliasDelta(unsigned bin) {
    unsigned bits = log2(bin) + 1;
    vector<unsigned> gammaBits = eliasGamma(bits);
    vector<unsigned> result;

    for (int i = 0; i < bits - 1; i++) {
        result.insert(result.begin(), (bin >> i) & 1);
    }

    // Я сделал наоборот: дописал закодированное количество бит слева
    // от числа без старшей единицы
    result.insert(result.begin(), gammaBits.begin(), gammaBits.end());
    return result;
}

/*
 * Алгоритм декодирования дельта-кода:
 * - Считаем количество нулей до первой единицы (М)
 * - Читаем следующие М бит. Получаем количество бит
 *   исходного числа + 1 (L).
 * - Читаем следующие L - 1 бит и восстанавливаем число.
 * */
vector<unsigned> deltaDecode(vector<unsigned> delta) {
    int bitZeros = 0;
    vector<unsigned>::iterator it = delta.begin();
    while (*it != 1) {
        bitZeros++;
        it++;
    }

    // Я просто обрежу M*2+1 бит с начала, что оставит только корректные биты исходного числа.
    // Например:
    // 00100111 -> 111
    vector<unsigned> slice = vector<unsigned>(delta.begin() + bitZeros * 2 + 1, delta.end());

    // Вставляем обрезанную старшую единицу: 111 -> 1111.
    slice.insert(slice.begin(), 1);
    return slice;
}

/*
 * Алгоритм построения омега-кода:
 * - В конец представления записать 0.
 * - Если число != 1, слева добавить двоичное представление числа
 * - В N записать новое значение – количество только что записанных бит
 *   минус 1.
 * - Вернуться к шагу 2.
 * */

vector<unsigned> eliasOmega(unsigned bin) {
    vector<unsigned> result;
    result.push_back(0);
    
    unsigned n = bin;
    while (n != 1) {
        unsigned bits = log2(n) + 1;
        for (int i = 0; i < bits; i++) {
            result.insert(result.begin(), (n >> i) & 1);
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

vector<unsigned> omegaDecode(vector<unsigned> omega) {
    vector<unsigned>::iterator it = omega.begin();
    vector<unsigned> tmp;
    unsigned n = 1;

    // Если исходное число – единица, то омега-код – просто нуль.
    // Как следствие, в результате получится пустая строка.
    // Чтобы этого не произошло, возвращаем единицу до начала выполнения
    // алгоритма.
    if (omega.size() == 1) {
        tmp.push_back(n);
        return tmp;
    }

    while (*it == 1) {
        tmp = vector<unsigned>(it, it + n + 1);
        it += n + 1;
        n = 0;

        for (int i = tmp.size() - 1; i >= 0; i--) {
            n += tmp[i] * (1 << (tmp.size() - i - 1));
        }
    }

    return tmp;
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
    for (int i = 1; i < 16; i++) {
        vector<unsigned> gamma = eliasGamma(i);
        vector<unsigned> gback = gammaDecode(gamma);
        vector<unsigned> delta = eliasDelta(i);
        vector<unsigned> dback = deltaDecode(delta);
        vector<unsigned> omega = eliasOmega(i);
        vector<unsigned> oback = omegaDecode(omega);

        cout << "Число: " << i << "\n" 
            << "Гамма-код: " << vecToString(gamma) << "\n" 
            << "Возврат из гамма-кода: " << vecToString(gback) << "\n"
            << "Дельта-код: " << vecToString(delta) << "\n" 
            << "Возврат из дельта-кода: " << vecToString(dback) << "\n" 
            << "Омега-код: " << vecToString(omega) << "\n"
            << "Возврат из омега-кода: " << vecToString(oback) << "\n"
            << endl;
    }

    return 0;
}

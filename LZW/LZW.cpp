/*
 * Программа для сжатия файлов с использованием алгоритма LZW.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 10 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

// Класс-фасад перед потоком вывода, позволяющий
// группировать биты кодов в байты
class output_code_stream {
public:
    output_code_stream(ostream& output, unsigned max_code)
        : m_output(output),
          m_code_size(9),
          m_pending_bits(0),
          m_pending_output(0),
          m_current_code(256),
          m_next_bump(512),
          m_max_code(max_code)
    {}

    ~output_code_stream() {
        *this << 256;
        flush(0);
    }

    void operator<<(const unsigned& i) {
        m_pending_output |= i << m_pending_bits;
        m_pending_bits += m_code_size;
        flush(8);
        if (m_current_code < m_max_code) {
            m_current_code++;
            if (m_current_code == m_next_bump) {
                m_next_bump *= 2;
                m_code_size++;
            }
        }
    }

private:
    void flush(const int val) {
        while (m_pending_bits >= val) {
            m_output.put(m_pending_output & 0xff);
            m_pending_output >>= 8;
            m_pending_bits -= 8;
        }
    }

    ostream& m_output;
    int m_code_size;
    int m_pending_bits;
    unsigned m_pending_output;
    unsigned m_current_code;
    unsigned m_next_bump;
    unsigned m_max_code;
};

// Класс-фасад перед потоком ввода, позволяющий
// считывать сгруппированные биты кодов
class input_code_stream {
public :
    input_code_stream(istream& input, unsigned max_code)
        : m_input(input),
          m_code_size(9),
          m_available_bits(0),
          m_pending_input(0),
          m_current_code(256),
          m_next_bump(512),
          m_max_code(max_code)
    {}

    bool operator>>(unsigned& i)
    {
        while (m_available_bits < m_code_size) {
            char c;
            if (!m_input.get(c)) return false;
            m_pending_input |= (c & 0xff) << m_available_bits;
            m_available_bits += 8;
        }

        i = m_pending_input & ~(~0 << m_code_size);
        m_pending_input >>= m_code_size;
        m_available_bits -= m_code_size;
        if (m_current_code < m_max_code) {
            m_current_code++;
            if (m_current_code == m_next_bump) {
                m_next_bump *= 2;
                m_code_size++;
            }
        }

        return i != 256;
    }

private :
    istream& m_input;
    int m_code_size;
    int m_available_bits;
    unsigned m_pending_input;
    unsigned m_current_code;
    unsigned m_next_bump;
    unsigned m_max_code;
};

void compress(istream& input, ostream& output, const unsigned max_code = 32767) {
    output_code_stream out(output, max_code);

    map<string, int> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[string(1, (char) i)] = i;
    }

    int nextCode = 257;
    string word;
    char c;
    while (input >> c) {
        word += c;
        if (dictionary.find(word) == dictionary.end()) {
            if (nextCode <= max_code) dictionary[word] = nextCode++;
            word.erase(word.size() - 1);
            out << dictionary[word];
            word = string(1, c);
        }
    }

    if (!word.empty()) out << dictionary[word];
}

void decompress(istream& input, ostream& output, const unsigned int max_code = 32767) {
    input_code_stream in(input, max_code);

    map<int, string> dictionary;
    for (int i = 0; i < 256; i++) {
        dictionary[i] = string(1, (char) i);
    }

    string word;
    unsigned code;
    int nextCode = 257;
    while (in >> code) {
        if (dictionary.find(code) == dictionary.end()) dictionary[code] = word + word[0];
        output << dictionary[code];
        if (!word.empty() && nextCode <= max_code) dictionary[nextCode++] = word + dictionary[code][0];
        word = dictionary[code];
    }
}

// Хэш-функция нужна для получения возможности использовать
// строки в конструкции switch..case
constexpr unsigned hashFn(const char* s, int off = 0) { 
    return !s[off] ? 5381 : (hashFn(s, off + 1) * 33) ^ s[off];
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " [compress|decompress] input_file output_file" << endl;
        return 0;
    }

    ifstream input;
    ofstream output;
    switch (hashFn(argv[1])) {
        case hashFn("compress"): {
            input.open(argv[2], ios::in);
            output.open(argv[3], ios::out | ios::binary);
            compress(input, output);
            break;
        }

        case hashFn("decompress"): {
            input.open(argv[2], ios::in | ios::binary);
            output.open(argv[3], ios::out);
            decompress(input, output);
            break;
        }

        default: {
            cout << "Usage: " << argv[0] << " [compress|decompress] input_file output_file" << endl;
            return 0;
        }
    }

    input.close();
    output.close();
    return 0;
}

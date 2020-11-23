/*
 * Демонстрация реализации кода Хэмминга для корректирования одиночной ошибки
 * и обнаружения двойной.
 *
 * Автор: Беспалов В. (3 курс, ИС)
 * Эффективное время написания: 16 часов
 * Компилятор: Apple LLVM version 10.0.0 (clang-1000.10.44.4)
 * Доп.флаги компиляции: -std=c++11
 */

#include <iostream>
#include <vector>
#include <functional> // для типа function
#include <cmath> // для floor, ceil, log2
#include <cassert> // для assert

using namespace std;

namespace HammingECC {
    function<unsigned()> _makePowersOf2Generator() {
        return []{
            unsigned power = 1;
            return [=]() mutable {
                unsigned prevPower = power;
                power <<= 1;
                return prevPower;
            };
        }();
    }

    unsigned _nextPowerOfTwo(unsigned n) {
        if ((n & (n - 1)) == 0) {
            return n << 1;
        }

        unsigned z = ceil(log2(n));
        return 1 << z;
    }

    unsigned _parityBitsRequired(unsigned length) {
        unsigned n = _nextPowerOfTwo(length);
        unsigned lowerBin = floor(log2(n));
        unsigned upperBin = lowerBin + 1;
        unsigned dataBitBoundary = n - lowerBin - 1;
        return length <= dataBitBoundary ? lowerBin : upperBin;
    }

    vector<unsigned> _dataBitsCovered(unsigned parity, unsigned lim) {
        vector<unsigned> result;
        unsigned dataIndex = 1, totalIndex = 3;
        while (dataIndex <= lim) {
            unsigned currBitIsData = totalIndex & (totalIndex - 1) ? 1 : 0;
            if (currBitIsData && (totalIndex % (parity << 1)) >= parity) {
                result.push_back(dataIndex - 1);
            }

            dataIndex += currBitIsData;
            totalIndex++;
        }

        return result;
    }

    unsigned _calculateParity(vector<unsigned> data, unsigned parity) {
        unsigned result = 0;
        if (parity == 0) {
            for (unsigned bit : data) { result ^= bit; } 
        } else {
            for (unsigned dataIndex : _dataBitsCovered(parity, data.size())) {
                result ^= data[dataIndex]; 
            }
        }

        return result;
    }

    vector<unsigned> encode(vector<unsigned> data) {
        unsigned dataLength = data.size();
        unsigned numParityBits = _parityBitsRequired(dataLength);
        unsigned encodedLength = dataLength + numParityBits + 1;

        vector<unsigned> encoded(encodedLength, 0);
        
        unsigned parityBitIndex;
        function<unsigned()> _powersOfTwo = _makePowersOf2Generator();
        while ((parityBitIndex = _powersOfTwo()) < (1 << numParityBits)) {
            encoded[parityBitIndex] = _calculateParity(data, parityBitIndex);
        }

        unsigned dataIndex = 0;
        for (unsigned encodedIndex = 3; encodedIndex < encodedLength; encodedIndex++) {
            if (encodedIndex & (encodedIndex - 1)) {
                encoded[encodedIndex] = data[dataIndex];
                dataIndex++;
            }
        }

        encoded[0] = _calculateParity(vector<unsigned>(encoded.begin() + 1, encoded.end()), 0);
        return encoded;
    }

    vector<unsigned> _extractData(vector<unsigned> encoded) {
        vector<unsigned> data;
        for (unsigned i = 3; i < encoded.size(); i++) {
            if (i & (i - 1)) {
                data.push_back(encoded[i]);
            }
        }

        return data;
    }

    vector<unsigned> decode(vector<unsigned> encoded) {
        unsigned encodedLength = encoded.size();
        unsigned numParityBits = floor(log2(encodedLength - 1)) + 1;
        unsigned indexOfError = 0;

        vector<unsigned> decoded = _extractData(encoded);
        unsigned overallExpected = _calculateParity(vector<unsigned>(encoded.begin() + 1, encoded.end()), 0);
        unsigned overallActual = encoded[0];

        unsigned parityBitIndex;
        function<unsigned()> _powersOfTwo = _makePowersOf2Generator();
        while ((parityBitIndex = _powersOfTwo()) < (1 << numParityBits)) {
            unsigned expected = _calculateParity(decoded, parityBitIndex);
            unsigned actual = encoded[parityBitIndex];
            if (expected != actual) {
                indexOfError += parityBitIndex;
            }
        }

        if (indexOfError && overallExpected == overallActual) {
            throw runtime_error("Double error detected!");
        } else if (indexOfError && overallExpected != overallActual) {
            encoded[indexOfError] ^= 1;
        } 

        decoded = _extractData(encoded);
        return decoded;
    }
};

int main(void) {
    vector<unsigned> data {0, 1, 0, 1, 0, 1, 0};
    vector<unsigned> encoded = HammingECC::encode(data);
    cout << "Testing no-error conversion... ";
    assert(data == HammingECC::decode(encoded));
    cout << "Passed!" << endl;

    for (unsigned i = 0; i < encoded.size(); i++) {
        vector<unsigned> encodedCopy = encoded;
        cout << "Testing converstion with error at index: " << i << "... ";
        encodedCopy[i] ^= 1;
        assert(data == HammingECC::decode(encodedCopy));
        cout << "Passed!" << endl;
    }

    cout << "Testing double error detection... ";
    try {
        vector<unsigned> encodedCopy = encoded;
        encodedCopy[0] ^= 1;
        encodedCopy[1] ^= 1;
        HammingECC::decode(encodedCopy);
    } catch (runtime_error e) {
        cout << e.what() << endl;
    }

    return 0;
}

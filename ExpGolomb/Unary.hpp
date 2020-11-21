#include <string>
#include <cassert> // для assert

using namespace std;

namespace Unary {
    string encode(unsigned n) {
        assert(n >= 0 && "Negative number detected");
        if (n == 0) return "0";
        else return "1" + encode(n - 1);
    }
};


#include <data/types.hpp>

#include <chrono>

namespace data {
    random_engine& get_random_engine () {
        static random_engine Engine;
        static bool Seeded = false;
        if (!Seeded) Engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
        return Engine;
    }
    
}

#if defined(__SIZEOF_INT128__)
std::ostream &print (std::ostream &os, __int128_t unsigned val) {
    if (val == 0)
        return os << '0';
    std::vector<std::uint16_t> digits;
    __int128_t unsigned value = val;
    while (value > 0) {
        digits.push_back (value % 10);
        value /= 10;
    }
    for (auto b = digits.rbegin (); b != digits.rend (); b++) os << *b;
    return os;
}

std::ostream &print (std::ostream &os, __int128_t val) {
    if (val == 0) return os << '0';

    __int128_t value = val;
    if (val < 0) os << "-";
    value = -value;
    std::vector<std::uint16_t> digits;
    while (value > 0) {
        digits.push_back (value % 10);
        value /= 10;
    }
    for (auto b = digits.rbegin (); b != digits.rend (); b++) os << *b;
    return os;
}
#endif

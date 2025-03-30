// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC_NEGATIVITY
#define DATA_ARITHMETIC_NEGATIVITY

#include <ostream>

namespace data::arithmetic {
    
    enum class negativity {
        nones = 0,
        // ones = 1
        twos = 2,
        BC = 3
    };

    template <negativity x>
    struct signed_negativity {
        static const bool value = true;
    };

    template <>
    struct signed_negativity<negativity::nones> {
        static const bool value = false;
    };
    
    std::ostream inline &operator << (std::ostream &o, negativity c) {
        switch (c) {
            default: return o << "none's negativity";
            // case negativity::ones: return o << "one's complement";
            case negativity::twos: return o << "two's complement";
            case negativity::BC: return o << "Bitcoin sign-and-magnetude";
        }
    }
}

namespace data {

    template <typename X> bool is_negative_zero (const X &x);
    template <typename X> bool is_positive_zero (const X &x);
}

namespace data::math {
    using negativity = arithmetic::negativity;

    template <typename X> struct is_negative_zero;
    template <typename X> struct is_positive_zero;
    
}

namespace data {

    template <typename X> bool inline is_negative_zero (const X &x) {
        return math::is_negative_zero<X> {} (x);
    }

    template <typename X> bool inline is_positive_zero (const X &x) {
        return math::is_positive_zero<X> {} (x);
    }

}

#endif

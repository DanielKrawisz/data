// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARITHMETIC_COMPLEMENT
#define DATA_ARITHMETIC_COMPLEMENT

#include <ostream>

namespace data::arithmetic {
    
    enum class complement {
        nones = 0, 
        ones = 1, 
        //twos = 2,
        BC = 3
    };

    template <complement x>
    struct signed_complement {
        static const bool value = true;
    };

    template <>
    struct signed_complement<complement::nones> {
        static const bool value = false;
    };
    
    std::ostream inline &operator << (std::ostream &o, complement c) {
        switch (c) {
            default: return o << "none's complement"; 
            case complement::ones: return o << "one's complement";
            //case complement::twos: return o << "two's complement";
            case complement::BC: return o << "Bitcoin";
        }
    }
}

namespace data::math {

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

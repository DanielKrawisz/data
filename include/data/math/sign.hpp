// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/types.hpp>

namespace data::math {
            
    enum sign : int8_t { zero = 0 , positive = 1 , negative = -1 };

    sign inline operator * (sign a, sign b) {
        return sign (int8_t (a) * int8_t (b));
    }
    
    std::ostream inline &operator << (std::ostream &o, sign x) {
        switch (x) {
            default: return o << "zero";
            case positive: return o << "positive";
            case negative: return o << "negative";
        }
    }
    
    template <std::unsigned_integral X>
    bool inline constexpr is_zero (X u) {
        return u == 0u;
    }
    
    template <std::signed_integral X>
    bool inline constexpr is_zero (X u) {
        return u == 0;
    }
    
    template <std::unsigned_integral X>
    bool inline constexpr is_positive (X u) {
        return u > 0u;
    }
    
    template <std::signed_integral X>
    bool inline constexpr is_positive (X u) {
        return u > 0;
    }
    
    template <std::unsigned_integral X>
    bool inline constexpr is_negative (X u) {
        return false;
    }
    
    template <std::signed_integral X>
    bool inline constexpr is_negative (X u) {
        return u < 0;
    }

}

namespace data {
    template <typename X> requires std::integral<X> || std::floating_point<X> 
    math::sign inline sign (const X& x) {
        return x == 0 ? math::zero : x > 0 ? math::positive : math::negative;
    }
    
    template <typename X> requires requires (const X x) {
        { x.sign () } -> std::convertible_to<math::sign>;
    } math::sign inline sign(const X& x) {
        return x.sign ();
    }
}

#endif

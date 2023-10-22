// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_ABS
#define DATA_MATH_NUMBER_ABS

#include <data/types.hpp>

namespace data::math {
    
    template <typename X> struct abs;
    template <typename X> struct quadrance;
}

namespace data {
    template <typename X> 
    auto abs (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::abs<X> {} (x);
    }
    
    template <typename X>
    auto quadrance (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::quadrance<X> {} (x);
    }
    
    template <typename X> auto inline norm (const X &x) -> decltype (quadrance (x)) {
        return quadrance (x);
    }
}

namespace data::math {
    
    template <std::unsigned_integral X> struct abs<X> {
        X operator () (const X &x) {
            return x;
        }
    };
    
    template <std::signed_integral X> struct abs<X> {
        std::make_unsigned<X>::type operator () (const X &x) {
            return static_cast<std::make_unsigned<X>::type> (x < 0 ? -x : x);
        }
    };
    
    template <std::unsigned_integral X> struct quadrance<X> {
        X operator () (const X &x) {
            return x * x;
        }
    };
    
    template <std::signed_integral X> struct quadrance<X> {
        std::make_unsigned<X>::type operator () (const X &x) {
            return data::abs (x) * data::abs (x);
        }
    };
    
}

#endif

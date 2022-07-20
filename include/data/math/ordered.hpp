// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ORDERED
#define DATA_MATH_ORDERED

#include <type_traits>

#include <data/types.hpp>

namespace data {
    
    template <typename X> concept ordered = std::totally_ordered<X>;
    
    template <ordered X> inline bool equal(const X& a, const X& b);
    template <ordered X> inline bool unequal(const X& a, const X& b);
    template <ordered X> inline bool less_equal(const X& a, const X& b);
    template <ordered X> inline bool greater_equal(const X& a, const X& b);
    template <ordered X> inline bool less(const X& a, const X& b);
    template <ordered X> inline bool greater(const X& a, const X& b);
    
}

namespace data::math {
    template <ordered X> struct equal {
        bool operator()(const X& a, const X& b) {
            return a == b;
        }
    };
    
    template <ordered X> struct unequal {
        bool operator()(const X& a, const X& b) {
            return a != b;
        }
    };
    
    template <ordered X> struct greater {
        bool operator()(const X& a, const X& b) {
            return a > b;
        }
    };
    
    template <ordered X> struct less {
        bool operator()(const X& a, const X& b) {
            return a < b;
        }
    };
    
    template <ordered X> struct greater_equal {
        bool operator()(const X& a, const X& b) {
            return a >= b;
        }
    };
    
    template <ordered X> struct less_equal {
        bool operator()(const X& a, const X& b) {
            return a <= b;
        }
    };
    
}

namespace data {
    
    template <ordered X> inline bool equal(const X& a, const X& b) {
        return math::equal<X>{}(a, b);
    }
    
    template <ordered X> inline bool unequal(const X& a, const X& b) {
        return math::unequal<X>{}(a, b);
    }
    
    template <ordered X> inline bool less_equal(const X& a, const X& b) {
        return math::less_equal<X>{}(a, b);
    }
    
    template <ordered X> inline bool greater_equal(const X& a, const X& b) {
        return math::greater_equal<X>{}(a, b);
    }
    
    template <ordered X> inline bool less(const X& a, const X& b) {
        return math::less<X>{}(a, b);
    }
    
    template <ordered X> inline bool greater(const X& a, const X& b) {
        return math::greater<X>{}(a, b);
    }
    
}

#endif


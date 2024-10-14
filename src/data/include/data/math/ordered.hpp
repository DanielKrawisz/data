// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ORDERED
#define DATA_MATH_ORDERED

#include <type_traits>

#include <data/concepts.hpp>

namespace data {
    
    template <typename X> concept ordered = std::totally_ordered<X>;
    template <typename X> concept sortable = requires (const X &a, const X &b) {
        {a < b} -> implicitly_convertible_to<bool>;
    };
    
    template <ordered X> bool equal (const X &a, const X &b);
    template <ordered X> bool unequal (const X &a, const X &b);
    template <ordered X> bool less_equal (const X &a, const X &b);
    template <ordered X> bool greater_equal (const X &a, const X &b);
    template <ordered X> bool less (const X &a, const X &b);
    template <ordered X> bool greater (const X &a, const X &b);
    template <ordered X> const X &max (const X &a, const X &b);
    template <ordered X> const X &min (const X &a, const X &b);
    
}

namespace data::math {
    template <ordered X> struct equal {
        bool operator () (const X &a, const X &b) {
            return a == b;
        }
    };
    
    template <ordered X> struct unequal {
        bool operator () (const X &a, const X &b) {
            return a != b;
        }
    };
    
    template <ordered X> struct greater {
        bool operator () (const X &a, const X &b) {
            return a > b;
        }
    };
    
    template <ordered X> struct less {
        bool operator () (const X & a, const X &b) {
            return a < b;
        }
    };
    
    template <ordered X> struct greater_equal {
        bool operator () (const X& a, const X& b) {
            return a >= b;
        }
    };
    
    template <ordered X> struct less_equal {
        bool operator () (const X &a, const X &b) {
            return a <= b;
        }
    };

    template <ordered X> struct min {
        const X &operator () (const X &a, const X &b) {
            return std::min (a, b);
        }
    };

    template <ordered X> struct mmax {
        const X &operator () (const X & a, const X &b) {
            return std::max (a, b);
        }
    };
    
}

namespace data {
    
    template <ordered X> bool inline equal (const X &a, const X &b) {
        return math::equal<X> {} (a, b);
    }
    
    template <ordered X> bool inline unequal (const X &a, const X &b) {
        return math::unequal<X> {} (a, b);
    }
    
    template <ordered X> bool inline less_equal (const X& a, const X &b) {
        return math::less_equal<X> {} (a, b);
    }
    
    template <ordered X> bool inline greater_equal (const X& a, const X &b) {
        return math::greater_equal<X> {} (a, b);
    }
    
    template <ordered X> bool inline less (const X &a, const X &b) {
        return math::less<X> {} (a, b);
    }
    
    template <ordered X> bool inline greater (const X &a, const X &b) {
        return math::greater<X> {} (a, b);
    }

    template <ordered X> const X inline &min (const X &a, const X &b) {
        return math::less<X> {} (a, b);
    }

    template <ordered X> const X inline &max (const X &a, const X &b) {
        return math::greater<X> {} (a, b);
    }
    
}

#endif


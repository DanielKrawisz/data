// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/concepts.hpp>
#include <data/types.hpp>
namespace data::math {

    enum signature : int8_t { zero = 0 , positive = 1 , negative = -1 };

    constexpr signature inline operator * (signature a, signature b);

    std::ostream inline &operator << (std::ostream &o, signature x);
}

namespace data {

    template <typename X> constexpr math::signature sign (const X &x);

    template <typename X> constexpr bool is_zero (const X &x);

    template <typename X> constexpr bool is_positive (const X &x);

    template <typename X> constexpr bool is_negative (const X &x);

}

namespace data::math {
    template <typename X> struct sign;

    template <typename X> struct is_zero;

    template <typename X> struct is_positive;

    template <typename X> struct is_negative;
}


namespace data {
    template <typename X> constexpr math::signature inline sign (const X &x) {
        return math::sign<X> {} (x);
    }

    template <typename X> constexpr bool inline is_zero (const X &x) {
        return math::is_zero<X> {} (x);
    }

    template <typename X> constexpr bool inline is_positive (const X &x) {
        return math::is_positive<X> {} (x);
    }

    template <typename X> constexpr bool inline is_negative (const X &x) {
        return math::is_negative<X> {} (x);
    }
}

namespace data::math {

    constexpr signature inline operator * (signature a, signature b) {
        return signature (int8_t (a) * int8_t (b));
    }
    
    std::ostream inline &operator << (std::ostream &o, signature x) {
        switch (x) {
            default: return o << std::string {"zero"};
            case positive: return o << std::string {"positive"};
            case negative: return o << std::string {"negative"};
        }
    }

    template <typename X>
    struct sign {
        constexpr math::signature operator () (const X &x) {
            return x == 0 ? math::zero : x > 0 ? math::positive : math::negative;
        }
    };

    template <std::unsigned_integral X>
    struct sign<X> {
        constexpr math::signature operator () (const X &x) {
            return x == 0 ? math::zero : math::positive;
        }
    };

    template <typename X> requires requires (const X &x) {
        { x.sign () } -> implicitly_convertible_to<signature>;
    } struct sign<X> {
        constexpr math::signature operator () (const X &x) {
            return x.sign ();
        }
    };

    template <typename X>
    struct is_zero {
        constexpr bool operator () (const X &x) {
            return x == 0;
        }
    };

    template <typename X>
    struct is_positive {
        constexpr bool operator () (const X &x) {
            return x > 0;
        }
    };

    template <typename X>
    struct is_negative {
        constexpr bool operator () (const X &x) {
            return x < 0;
        }
    };

    template <std::unsigned_integral X>
    struct is_negative<X> {
        constexpr bool operator () (const X &x) {
            return false;
        }
    };

    template <typename X> concept has_sign_function = requires (const X &x) {
        {data::sign (x)} -> implicitly_convertible_to<signature>;
    };

}

#endif

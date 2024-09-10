// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/concepts.hpp>
#include <data/types.hpp>

namespace data::math {

    enum signature : int8_t { zero = 0 , positive = 1 , negative = -1 };

    signature constexpr inline operator * (signature a, signature b) {
        return signature (int8_t (a) * int8_t (b));
    }
    
    std::ostream inline &operator << (std::ostream &o, signature x) {
        switch (x) {
            default: return o << "zero";
            case positive: return o << "positive";
            case negative: return o << "negative";
        }
    }

    template <typename X> struct sign;
    template <typename X> struct is_zero;
    template <typename X> struct is_positive;
    template <typename X> struct is_negative;

    template <typename X> requires std::integral<X> || std::floating_point<X>
    struct sign<X> {
        math::signature constexpr operator () (const X &x) {
            return x == 0 ? math::zero : x > 0 ? math::positive : math::negative;
        }
    };

    template <typename X> requires requires (const X &x) {
        { x.sign () } -> convertible_to<signature>;
    } struct sign<X> {
        math::signature constexpr operator () (const X &x) {
            return x.sign ();
        }
    };

    template <typename X>  requires requires (const X x) {
        {x == 0} -> convertible_to<bool>;
    } struct is_zero<X> {
        bool constexpr operator () (const X &x) {
            return x == 0;
        }
    };

    template <typename X> requires requires (const X x) {
        {x > 0} -> convertible_to<bool>;
    } struct is_positive<X> {
        bool constexpr operator () (const X &x) {
            return x > 0;
        }
    };

    template <typename X>  requires requires (const X x) {
        {x < 0} -> convertible_to<bool>;
    } struct is_negative<X> {
        bool constexpr operator () (const X &x) {
            return x < 0;
        }
    };

    template <typename X>
    struct sign {
        signature operator () (const X &x) {
            return is_negative<X> {} (x) ? negative : is_positive<X> {} (x) ? positive : zero;
        }
    };

}

namespace data {
    template <typename X> requires requires (const X x) {
        { math::sign<X> {} (x) } -> convertible_to<math::signature>;
    } math::signature inline sign (const X &x) {
        return math::sign<X> {} (x);
    }

    template <typename X> requires requires (const X x) {
        { math::is_zero<X> {} (x) } -> convertible_to<bool>;
    } bool inline is_zero (const X &x) {
        return math::is_zero<X> {} (x);
    }

    template <typename X> requires requires (const X x) {
        { math::is_positive<X> {} (x) } -> convertible_to<bool>;
    } bool inline is_positive (const X &x) {
        return math::is_positive<X> {} (x);
    }

    template <typename X> requires requires (const X x) {
        { math::is_negative<X> {} (x) } -> convertible_to<bool>;
    } bool inline is_negative (const X &x) {
        return math::is_negative<X> {} (x);
    }
}

namespace data::math {
            
    template <typename X> concept has_sign_function = requires (const X &x) {
        {data::sign (x)} -> convertible_to<signature>;
    };

}

#endif

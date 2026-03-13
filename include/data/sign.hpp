// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/types.hpp>
#include <data/ordered.hpp>

namespace data::math {

    enum sign : int8_t { zero = 0 , positive = 1 , negative = -1 };

    constexpr sign inline operator * (sign a, sign b);
    constexpr sign inline operator - (sign a);

    std::ostream inline &operator << (std::ostream &o, sign x);
}

namespace data {

    template <typename X> constexpr math::sign sign (const X &x);

    template <typename X> constexpr bool is_zero (const X &x);

    template <typename X> constexpr bool is_positive (const X &x);

    template <typename X> constexpr bool is_negative (const X &x);

    template <typename N, typename M> concept comparable_to =
        requires (const N &n, const M &m) {
            { n == m } -> Same<bool>;
            { n != m } -> Same<bool>;
            { n > m } -> Same<bool>;
            { n < m } -> Same<bool>;
            { n >= m } -> Same<bool>;
            { n <= m } -> Same<bool>;
            { m == n } -> Same<bool>;
            { m != n } -> Same<bool>;
            { m > n } -> Same<bool>;
            { m < n } -> Same<bool>;
            { m > n } -> Same<bool>;
            { m <= n } -> Same<bool>;
            { m >= n } -> Same<bool>;
        };

    template <typename X> concept NumberComparableSigned =
        Ordered<X> && requires (const X &x) {
            { sign (x) };
            { is_zero (x) };
            { is_positive (x) };
            { is_negative (x) };
        } && comparable_to<X, int> &&
        comparable_to<X, long> &&
        comparable_to<X, long long>;

    template <typename X> concept NumberComparable =
        NumberComparableSigned<X> &&
        comparable_to<X, unsigned int> &&
        comparable_to<X, unsigned long> &&
        comparable_to<X, unsigned long long>;

}

namespace data::math::def {
    template <typename X> struct sign;

    template <typename X> struct is_zero;

    template <typename X> struct is_positive;

    template <typename X> struct is_negative;
}


namespace data {
    template <typename X> constexpr math::sign inline sign (const X &x) {
        return math::def::sign<X> {} (x);
    }

    template <typename X> constexpr bool inline is_zero (const X &x) {
        return math::def::is_zero<X> {} (x);
    }

    template <typename X> constexpr bool inline is_positive (const X &x) {
        return math::def::is_positive<X> {} (x);
    }

    template <typename X> constexpr bool inline is_negative (const X &x) {
        return math::def::is_negative<X> {} (x);
    }

    template <typename X> concept has_sign_function = requires (const X &x) {
        {data::sign (x)} -> ImplicitlyConvertible<math::sign>;
    };
}

namespace data::math {

    constexpr sign inline operator * (sign a, sign b) {
        return sign (int8 (a) * int8 (b));
    }

    constexpr sign inline operator - (sign a) {
        return sign (int8 (-int8 (a)));
    }
    
    std::ostream inline &operator << (std::ostream &o, sign x) {
        switch (x) {
            default: return o << std::string {"zero"};
            case positive: return o << std::string {"positive"};
            case negative: return o << std::string {"negative"};
        }
    }
}

namespace data::math::def {

    template <typename X>
    struct sign {
        constexpr math::sign operator () (const X &x) {
            return x == 0 ? math::zero : x > 0 ? math::positive : math::negative;
        }
    };

    template <std::unsigned_integral X>
    struct sign<X> {
        constexpr math::sign operator () (const X &x) {
            return x == 0 ? math::zero : math::positive;
        }
    };

    template <typename X> requires requires (const X &x) {
        { x.sign () } -> ImplicitlyConvertible<math::sign>;
    } struct sign<X> {
        constexpr math::sign operator () (const X &x) {
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

}

#endif

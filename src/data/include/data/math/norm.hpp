// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NORM
#define DATA_MATH_NORM

#include <data/math/abs.hpp>

namespace data {

    template <typename A> auto norm (const A &);

    template <typename A> auto quadrance (const A &);

    template <typename A> A conjugate (const A &);

    template <typename A> auto re (const A &);

    template <typename A> auto im (const A &);

    template <typename A, typename B = A> auto quadrance (const A &, const B &);

    template <typename F, typename Q> concept normed = ordered<Q> && requires (const F &x) {
        { norm (x) } -> same_as<Q>;
    };

    template <typename X> concept conjugable = requires (const X &x) {
        { conjugate (x) } -> same_as<X>;
    };
}

namespace data::math {

    template <typename X> struct norm;
    template <typename X> struct quadrance;
    template <typename X> struct conjugate;
    template <typename X> struct re;
    template <typename X> struct im;
    template <typename X> struct inner;

    // if a quadrance is defined, then norm is defined in terms of quadrance.
    template <typename X> requires requires (const X &x) {
        { quadrance<X> {} (x) };
    } struct norm<X> {
        auto operator () (const X &x) {
            return quadrance<X> {} (x);
        }
    };

    // if re and inner are defined, then quadrance is defined in terms of them.
    template <typename X> requires requires (const X &x) {
        { re<X> {} (x) };
        { inner<X> {} (x) };
    } struct quadrance<X> {
        auto operator () (const X &x) {
            return re<X> {} (inner<X> {} (x, x));
        }
    };

    // if conjugate is defined, then inner is defined in terms of it.
    template <typename X> requires requires (const X &x) {
        { conjugate<X> {} (x) };
    } struct inner<X> {
        auto operator () (const X &x, const X &y) {
            return x * conjugate<X> {} (y);
        }
    };
}

namespace data {

    template <typename X>
    auto inline quadrance (const X &x) {
        return math::quadrance<X> {} (x);
    }

    template <typename X>
    X inline conjugate (const X &x) {
        return math::conjugate<X> {} (x);
    }

    template <typename X>
    auto inline re (const X &x) {
        return math::re<X> {} (x);
    }

    template <typename X>
    auto inline im (const X &x) {
        return math::im<X> {} (x);
    }

    template <typename X> auto inline norm (const X &x) {
        return math::norm<X> {} (x);
    }

    template <typename X> auto inline inner (const X &x, const X &y) {
        return math::inner<X> {} (x, y);
    }
}

namespace data::math {

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

    template <std::floating_point X> struct re<X> {
        X operator () (const X &x) {
            return x;
        }
    };

    template <std::integral X> struct re<X> {
        X operator () (const X &x) {
            return x;
        }
    };

    template <std::floating_point X> struct im<X> {
        X operator () (const X &x) {
            return 0;
        }
    };

    template <std::integral X> struct im<X> {
        X operator () (const X &x) {
            return 0;
        }
    };

    template <std::signed_integral X> struct conjugate<X> {
        X operator () (const X &x) {
            return x;
        }
    };

    template <std::floating_point X> struct conjugate<X> {
        X operator () (const X &x) {
            return x;
        }
    };

    template <std::unsigned_integral X> struct inner<X> {
        X operator () (const X &x, const X &y) {
            return x * y;
        }
    };

}

#endif

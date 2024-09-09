// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ABS
#define DATA_MATH_ABS

#include <data/types.hpp>

namespace data::math {
    
    template <typename X> struct abs;
    template <typename X> struct quadrance;
    template <typename X> struct conjugate;
    template <typename X> struct re;
    template <typename X> struct im;

    template <typename X, typename F> concept normed = requires (const X &x) {
        {quadrance<X> {} (x)} -> std::same_as<F>;
    };

    template <typename X> concept conjugable = requires (const X &x) {
        {conjugate<X> {} (x)} -> std::same_as<X>;
    };
}

namespace data {
    template <typename X> 
    auto inline abs (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::abs<X> {} (x);
    }
    
    template <typename X>
    auto inline quadrance (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::quadrance<X> {} (x);
    }
    
    template <typename X> 
    X inline conjugate (const X &x) {
        return math::conjugate<X> {} (x);
    }
    
    template <typename X> 
    auto inline re (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::re<X> {} (x);
    }
    
    template <typename X> 
    auto inline im (const X &x) -> decltype (math::abs<X> {} (x)) {
        return math::im<X> {} (x);
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
    
}

#endif

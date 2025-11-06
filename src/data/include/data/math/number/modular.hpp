// Copyright (c) 2019-202 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_MODULAR
#define DATA_MATH_NUMBER_MODULAR

#include <data/types.hpp>
#include <data/integral.hpp>
#include <data/math/algebra.hpp>

namespace data::math::number {
    // TODO modular must satisfy group_integral
    template <auto mod, ring_number X = decltype (mod)> struct modular;
    
    template <auto mod, ring_number X = decltype (mod)>
    constexpr std::weak_ordering operator <=> (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    constexpr bool operator == (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    constexpr modular<mod, X> operator + (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    constexpr modular<mod, X> operator - (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    constexpr modular<mod, X> operator - (const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    modular<mod, X> operator * (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X = decltype (mod)>
    modular<mod, X> operator ^ (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, ring_number X> struct modular {
        X Value;

        template <typename... P>
        requires requires (P... p) {
            { X (p...) };
        } constexpr modular (P... p): Value (p...) {
            Value %= mod;
        }

        constexpr modular (const X &v): Value (v) {
            Value %= mod;
        }
        
        constexpr bool valid () const {
            return data::valid (Value) && Value >= 0 && Value < mod;;
        }

        constexpr operator X () const {
            return Value;
        }
        
    };

    template <auto mod, ring_number X> struct increment<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X>);
    };

    template <auto mod, ring_number X> struct decrement<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X>);
    };
    
}

namespace data::math {

    template <auto mod, typename X>
    struct numeric_limits<number::modular<mod, X>> {
        constexpr static const number::modular<mod, X> Max {mod - 1};
        constexpr static const number::modular<mod, X> Min {0};

        constexpr static const number::modular<mod, X> &max () {
            return Max;
        }

        constexpr static const number::modular<mod, X> &min () {
            return Min;
        }
    };
}

namespace data::math::def {
    
    template <auto mod, typename X>
    struct identity<plus<number::modular<mod, X>>, number::modular<mod, X>>
        : identity<plus<X>, X> {
        constexpr number::modular<mod, X> operator () () {
            return {identity<plus<X>, X>::value ()};
        }
    };
    
    template <auto mod, typename X>
    struct inverse<plus<number::modular<mod, X>>, number::modular<mod, X>> {
        constexpr number::modular<mod, X> operator () (const number::modular<mod, X> &a, const number::modular<mod, X> &b) {
            return minus_mod (b.Value, a.Value, nonzero {mod});
        }
    };
    
}

namespace data::math::number {

    template <auto mod, ring_number X = decltype (mod)>
    inline std::ostream &operator << (std::ostream &o, const data::math::number::modular<mod, X> &m) {
        return o << m.Value;
    }
    
    template <auto mod, ring_number X>
    constexpr bool inline operator == (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value == b.Value;
    }

    template <auto mod, ring_number X>
    constexpr std::weak_ordering inline operator <=> (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value <=> b.Value;
    }
    
    template <auto mod, ring_number X>
    constexpr modular<mod, X> inline operator + (const modular<mod, X> &a, const modular<mod, X> &b) {
        return data::plus_mod (a.Value, b.Value, nonzero {mod});
    }
    
    template <auto mod, ring_number X>
    modular<mod, X> inline operator * (const modular<mod, X> &a, const modular<mod, X> &b) {
        return data::times_mod (a.Value, b.Value, nonzero {mod});
    }
    
    template <auto mod, ring_number X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a, const modular<mod, X> &b) {
        if (a.Value < b.Value) return mod - (b.Value - a.Value);
        return data::minus_mod (a.Value, b.Value, nonzero {mod});
    }
    
    template <auto mod, ring_number X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a) {
        return data::negate_mod (a.Value, nonzero {mod});
    }
    
    template <auto mod, ring_number X>
    constexpr modular<mod, X> inline operator ^ (const modular<mod, X> &a, const X &b) {
        return data::pow_mod (a.Value, b, nonzero {mod});
    }

}

#endif


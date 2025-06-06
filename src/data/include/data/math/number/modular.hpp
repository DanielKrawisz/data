// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_MODULAR
#define DATA_MATH_NUMBER_MODULAR

#include <data/types.hpp>
#include <data/integral.hpp>
#include <data/math/algebra.hpp>

namespace data::math::number {
    // TODO modular must satisfy group_integral
    template <auto mod, typename X = decltype (mod)> struct modular;
    
    template <auto mod, typename X = decltype (mod)>
    constexpr std::weak_ordering operator <=> (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr bool operator == (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr modular<mod, X> operator + (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr modular<mod, X> operator - (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr modular<mod, X> operator - (const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr modular<mod, X> operator * (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    constexpr modular<mod, X> operator ^ (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X> struct modular {
        X Value;
        
        template <typename... P> constexpr modular (P... p);
        
        constexpr bool valid () const;

        constexpr operator X () const {
            return Value;
        }
        
    };

    template <auto mod, typename X> struct increment<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X>);
    };

    template <auto mod, typename X> struct decrement<modular<mod, X>> {
        constexpr modular<mod, X> operator () (const modular<mod, X>);
    };

    template <auto mod, typename X = decltype (mod)>
    inline std::ostream &operator << (std::ostream &o, const data::math::number::modular<mod, X> &m) {
        return o << m.Value;
    }
    
}

namespace data::math {
    
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
            return data::minus_mod (b.Value, a.Value);
        }
    };
    
}

namespace data::math::number {
    
    template <auto mod, typename X>
    constexpr bool inline operator == (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value == b.Value;
    }

    template <auto mod, typename X>
    constexpr std::weak_ordering inline operator <=> (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value <=> b.Value;
    }
    
    template <auto mod, typename X>
    constexpr modular<mod, X> inline operator + (const modular<mod, X> &a, const modular<mod, X> &b) {
        return data::plus_mod (a.Value, b.Value, nonzero {mod});
    }
    
    template <auto mod, typename X>
    constexpr modular<mod, X> inline operator * (const modular<mod, X> &a, const modular<mod, X> &b) {
        return data::times_mod (a.Value, b.Value, nonzero {mod});
    }
    
    template <auto mod, typename X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a, const modular<mod, X> &b) {
        if (a.Value < b.Value) return modular<mod, X>::modulus () - (b.Value - a.Value);
        return data::minus_mod (a.Value, b.Value, mod);
    }
    
    template <auto mod, typename X>
    constexpr modular<mod, X> inline operator - (const modular<mod, X> &a) {
        return data::negate_mod (a.Value, mod);
    }
    
    template <auto mod, typename X>
    constexpr modular<mod, X> inline operator ^ (const modular<mod, X> &a, const X &b) {
        return data::pow_mod (a.Value, b, mod);
    }
    
    template <auto mod, typename X>
    template <typename... P>
    constexpr inline modular<mod, X>::modular (P... p) : Value (p...) {
        Value %= mod;
    }
    
    template <auto mod, typename X>
    constexpr bool inline modular<mod, X>::valid () const {
        return Value >= 0 && Value < mod;
    }

}

#endif


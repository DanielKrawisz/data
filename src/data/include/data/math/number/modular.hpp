// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_MODULAR
#define DATA_MATH_NUMBER_MODULAR

#include <data/types.hpp>
#include <data/math/power.hpp>
#include <data/math/abs.hpp>
#include <data/math/algebra.hpp>

#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>

namespace data::math::number {
    
    template <auto mod, typename X = decltype (mod)> struct modular;
    
    template <auto mod, typename X = decltype (mod)>
    std::weak_ordering operator <=> (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    bool operator == (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    modular<mod, X> operator + (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    modular<mod, X> operator - (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    modular<mod, X> operator - (const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    modular<mod, X> operator * (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod, typename X = decltype (mod)>
    modular<mod, X> operator ^ (const modular<mod, X> &, const modular<mod, X> &);
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> operator + (const modular<mod, CryptoPP::Integer> &, const modular<mod, CryptoPP::Integer> &);
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> operator - (const modular<mod, CryptoPP::Integer> &, const modular<mod, CryptoPP::Integer> &);
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> operator * (const modular<mod, CryptoPP::Integer> &, const modular<mod, CryptoPP::Integer> &);
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> operator ^ (const modular<mod, CryptoPP::Integer> &, const modular<mod, CryptoPP::Integer> &);
    
    template <auto mod, typename X> struct modular {
        X Value;
        static const X &modulus ();
        
        template <typename... P> modular (P... p);
        
        bool valid () const;

        operator X () const;
        
    };
    
    template <auto mod> struct modular<mod, CryptoPP::Integer> {
        CryptoPP::Integer Value;
        static CryptoPP::ModularArithmetic &arithmetic ();
        static const CryptoPP::Integer &modulus ();
        
        template <typename... P> modular (P... p);
        
        bool valid () const;

        operator CryptoPP::Integer () const;
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
        number::modular<mod, X> operator () () {
            return {identity<plus<X>, X>::value ()};
        }
    };
    
    template <auto mod, typename X>
    struct inverse<plus<number::modular<mod, X>>, number::modular<mod, X>> {
        number::modular<mod, X> operator () (const number::modular<mod, X> &a, const number::modular<mod, X> &b) {
            return b - a;
        }
    };
    
}

namespace data::math::number {
    
    template <auto mod, typename X>
    bool inline operator == (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value == b.Value;
    }

    template <auto mod, typename X>
    std::weak_ordering inline operator <=> (const modular<mod, X> &a, const modular<mod, X> &b) {
        return a.Value <=> b.Value;
    }
    
    template <auto mod, typename X>
    modular<mod, X> inline operator + (const modular<mod, X> &a, const modular<mod, X> &b) {
        return (a.Value + b.Value) % modular<mod, X>::modulus ();
    }
    
    template <auto mod, typename X>
    modular<mod, X> inline operator * (const modular<mod, X> &a, const modular<mod, X> &b) {
        return (a.Value * b.Value) % modular<mod, X>::modulus ();
    }
    
    template <auto mod, typename X>
    modular<mod, X> inline operator - (const modular<mod, X> &a, const modular<mod, X> &b) {
        if (a.Value < b.Value) return modular<mod, X>::modulus () - (b.Value - a.Value);
        return a.Value - b.Value;
    }
    
    template <auto mod, typename X>
    modular<mod, X> inline operator - (const modular<mod, X> &a) {
        return {modular<mod, X>::modulus () - a.Value};
    }
    
    template <auto mod, typename X>
    modular<mod, X> inline operator ^ (const modular<mod, X> &a, const X &b) {
        return {pow_mod<X> {} (modular<mod, X>::modulus (), a.Value, b)};
    }
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> inline operator +
    (const modular<mod, CryptoPP::Integer> &a, const modular<mod, CryptoPP::Integer> &b) {
        return modular<mod, CryptoPP::Integer>::arithmetic ().Add (a.Value, b.Value);
    }
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> inline operator -
    (const modular<mod, CryptoPP::Integer> &a, const modular<mod, CryptoPP::Integer> &b) {
        return modular<mod, CryptoPP::Integer>::arithmetic ().Subject (a.Value, b.Value);
    }
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> inline operator *
    (const modular<mod, CryptoPP::Integer> &a, const modular<mod, CryptoPP::Integer> &b) {
        return modular<mod, CryptoPP::Integer>::arithmetic ().Multiply (a.Value, b.Value);
    }
    
    template <auto mod>
    modular<mod, CryptoPP::Integer> inline operator ^
    (const modular<mod, CryptoPP::Integer> &a, const CryptoPP::Integer &b) {
        modular<mod, CryptoPP::Integer> result;
        modular<mod, CryptoPP::Integer>::arithmetic ().CascadeExponentiate (&result.Value, a.Value, &b, 1);
        return result;
    }
    
    template <auto mod, typename X>
    const X &modular<mod, X>::modulus () {
        static X Mod (mod);
        return Mod;
    }
    
    template <auto mod, typename X>
    template <typename... P>
    inline modular<mod, X>::modular (P... p) : Value (p...) {
        Value %= modulus ();
    }
    
    template <auto mod, typename X>
    bool inline modular<mod, X>::valid () const {
        return Value >= 0 && Value < modulus ();
    }
    
    template <auto mod>
    CryptoPP::ModularArithmetic &modular<mod, CryptoPP::Integer>::arithmetic () {
        static CryptoPP::ModularArithmetic Mod {CryptoPP::Integer (mod)};
        return Mod;
    }
    
    template <auto mod>
    const CryptoPP::Integer inline &modular<mod, CryptoPP::Integer>::modulus () {
        return arithmetic ().GetModulus ();
    }
    
    template <auto mod>
    template <typename... P>
    inline modular<mod, CryptoPP::Integer>::modular (P... p) : Value (p...) {}
    
    template <auto mod>
    bool inline modular<mod, CryptoPP::Integer>::valid () const {
        return Value >= 0 && Value < arithmetic ().GetModulus ();
    }

}

#endif


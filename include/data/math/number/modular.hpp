// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_MODULAR
#define DATA_MATH_NUMBER_MODULAR

#include <data/types.hpp>
#include <data/math/number/natural.hpp>

#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>

namespace data::math::number {
    
    template <typename X, auto & mod> struct modular;
    
    template <typename X, auto & mod>
    bool inline operator==(const modular<X, mod> &, const modular<X, mod> &);
    
    template <typename X, auto & mod>
    bool inline operator!=(const modular<X, mod> &, const modular<X, mod> &);
    
    template <typename X, auto & mod>
    modular<X, mod> operator+(const modular<X, mod> &, const modular<X, mod> &);
    
    template <typename X, auto & mod>
    modular<X, mod> operator-(const modular<X, mod> &, const modular<X, mod> &);
    
    template <typename X, auto & mod>
    modular<X, mod> operator-(const modular<X, mod> &);
    
    template <typename X, auto & mod>
    modular<X, mod> operator*(const modular<X, mod> &, const modular<X, mod> &);
    
    template <typename X, auto & mod>
    modular<X, mod> operator^(const modular<X, mod> &, const modular<X, mod> &);
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> operator+(const modular<CryptoPP::Integer, mod> &, const modular<CryptoPP::Integer, mod> &);
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> operator-(const modular<CryptoPP::Integer, mod> &, const modular<CryptoPP::Integer, mod> &);
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> operator*(const modular<CryptoPP::Integer, mod> &, const modular<CryptoPP::Integer, mod> &);
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> operator^(const modular<CryptoPP::Integer, mod> &, const modular<CryptoPP::Integer, mod> &);
    
    template <typename X, auto & mod> struct modular {
        X Value;
        static const X &modulus();
        
        template <typename... P> modular(P... p);
        
        bool valid() const;
        
    };
    
    template <auto & mod> struct modular<CryptoPP::Integer, mod> {
        CryptoPP::Integer Value;
        static CryptoPP::ModularArithmetic& arithmetic();
        static const CryptoPP::Integer &modulus();
        
        template <typename... P> modular(P... p);
        
        bool valid() const;
        
    };

    template <typename X, auto & mod>
    inline std::ostream& operator<<(std::ostream& o, 
        const data::math::number::modular<X, mod>& m) {
        return o << m.Value;
    }
    
}

namespace data::math {
    
    template <typename X, auto & mod>
    struct commutative<plus<number::modular<X, mod>>, number::modular<X, mod>>
        : commutative<plus<X>, X> {};
    
    template <typename X, auto & mod>
    struct associative<data::plus<number::modular<X, mod>>, number::modular<X, mod>>
        : associative<data::plus<X>, X> {};
    
    template <typename X, auto & mod>
    struct identity<data::plus<number::modular<X, mod>>, number::modular<X, mod>>
        : identity<data::plus<X>, X> {
        static const number::modular<X, mod> value() {
            return {identity<data::plus<X>, X>::value()};
        }
    };
    
}

namespace data::math::number {
    
    template <typename X, auto & mod>
    bool inline operator==(const modular<X, mod> &a, const modular<X, mod> &b) {
        return a.Value == b.Value;
    }
    
    template <typename X, auto & mod>
    bool inline operator!=(const modular<X, mod> &a, const modular<X, mod> &b) {
        return a.Value != b.Value;
    }
    
    template <typename X, auto & mod>
    modular<X, mod> inline operator+(const modular<X, mod> &a, const modular<X, mod> &b) {
        return (a.Value + b.Value) % modular<X, mod>::modulus();
    }
    
    template <typename X, auto & mod>
    modular<X, mod> inline operator*(const modular<X, mod> &a, const modular<X, mod> &b) {
        return (a.Value * b.Value) % modular<X, mod>::modulus();
    }
    
    template <typename X, auto & mod>
    modular<X, mod> inline operator-(const modular<X, mod> &a, const modular<X, mod> &b) {        
        if (a.Value < b.Value) return modular<X, mod>::modulus() - (b.Value - a.Value);
        return a.Value - b.Value;
    }
    
    template <typename X, auto & mod>
    modular<X, mod> inline operator-(const modular<X, mod> &a) {
        return {modular<X, mod>::modulus() - a.Value};
    }
    
    template <typename X, auto & mod>
    modular<X, mod> inline operator^(const modular<X, mod> &a, const modular<X, mod> &b) {
        return (a.Value * b.Value) % modular<X, mod>::modulus();
    }
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> inline operator+(const modular<CryptoPP::Integer, mod> &a, const modular<CryptoPP::Integer, mod> &b) {
        return modular<CryptoPP::Integer, mod>::arithmetic().Add(a.Value, b.Value);
    }
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> inline operator-(const modular<CryptoPP::Integer, mod> &a, const modular<CryptoPP::Integer, mod> &b) {
        return modular<CryptoPP::Integer, mod>::arithmetic().Subject(a.Value, b.Value);
    }
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> inline operator*(const modular<CryptoPP::Integer, mod> &a, const modular<CryptoPP::Integer, mod> &b) {
        return modular<CryptoPP::Integer, mod>::arithmetic().Multiply(a.Value, b.Value);
    }
    
    template <auto & mod>
    modular<CryptoPP::Integer, mod> inline operator^(const modular<CryptoPP::Integer, mod> &a, const modular<CryptoPP::Integer, mod> &b) {
        modular<CryptoPP::Integer, mod> result;
        modular<CryptoPP::Integer, mod>::arithmetic().CascadeExponentiate(&result.Value, a.Value, &b.Value, 1);
        return result;
    }
    
    template <typename X, auto & mod>
    const X &modular<X, mod>::modulus() { 
        static X Mod(mod);
        return Mod;
    }
    
    template <typename X, auto & mod>
    template <typename... P>
    inline modular<X, mod>::modular(P... p) : Value(p...) {}
    
    template <typename X, auto & mod>
    bool inline modular<X, mod>::valid() const {
        return Value >= 0 && Value < modulus();
    }
    
    template <auto & mod>
    CryptoPP::ModularArithmetic &modular<CryptoPP::Integer, mod>::arithmetic() { 
        static CryptoPP::ModularArithmetic Mod{CryptoPP::Integer(mod)};
        return Mod;
    }
    
    template <auto & mod>
    const CryptoPP::Integer inline &modular<CryptoPP::Integer, mod>::modulus() { 
        return arithmetic().GetModulus();
    }
    
    template <auto & mod>
    template <typename... P>
    inline modular<CryptoPP::Integer, mod>::modular(P... p) : Value(p...) {}
    
    template <auto & mod>
    bool inline modular<CryptoPP::Integer, mod>::valid() const {
        return Value >= 0 && Value < arithmetic().GetModulus();
    }

}

#endif


// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_FINITE_FIELD
#define DATA_MATH_ALGEBRA_FINITE_FIELD

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/algebra.hpp>
#include <data/set.hpp>

namespace data::math {
    template <auto P, ring_number N = decltype (P)> struct prime_field;
    
    template <auto P, ring_number N>
    constexpr bool operator == (const prime_field<P, N> &, const prime_field<P, N> &);
    
    // NOTE: N must be a signed number because we have to
    // be able to take the mod of Bezout T, which may run
    // negative.
    template <auto P, ring_number N>
    struct prime_field : number::modular<P, N> {
        using number::modular<P, N>::modular;
        prime_field (const number::modular<P, N> &n): number::modular<P, N> {n} {}
        prime_field (number::modular<P, N> &&n): number::modular<P, N> {n} {}
    
        constexpr prime_field operator + (const prime_field &) const;
        constexpr prime_field operator - (const prime_field &) const;
        constexpr prime_field operator * (const prime_field &) const;
        constexpr prime_field operator / (const prime_field &) const;
        constexpr prime_field operator ~ () const;
        
        constexpr prime_field inverse () const;
        
    };

    template <auto P, ring_number N>
    set<prime_field<P, N>> square_root (prime_field<P, N>);

    template <auto P, ring_number N>
    std::ostream inline &operator << (std::ostream &o, const prime_field<P, N> &m) {
        return o << "f<" << P << "> {" << m.Value << "}";
    }

    namespace def {
        template <auto prime, ring_number N>
        struct times<prime_field<prime, N>> {
            constexpr prime_field<prime, N> operator () (const prime_field<prime, N> &a, const prime_field<prime, N> &b) {
                return a * b;
            }

            constexpr nonzero<prime_field<prime, N>> operator ()
            (const nonzero<prime_field<prime, N>> &a, const nonzero<prime_field<prime, N>> &b) {
                return nonzero<prime_field<prime, N>> {a.Value * b.Value};
            }
        };

        template <auto prime, ring_number N>
        struct identity<plus<prime_field<prime, N>>,
            prime_field<prime, N>>
            : identity<plus<N>, N> {
            prime_field<prime, N> operator () () {
                return {identity<plus<N>, N>::value ()};
            }
        };

        template <auto prime, ring_number N>
        struct identity<times<prime_field<prime, N>>, prime_field<prime, N>> : identity<times<N>, N> {
            constexpr prime_field<prime, N> operator () () {
                return {identity<times<N>, N>::value ()};
            }
        };

        template <auto prime, ring_number N>
        struct inverse<plus<prime_field<prime, N>>, prime_field<prime, N>> {
            constexpr prime_field<prime, N> operator () (const prime_field<prime, N> &a, const prime_field<prime, N> &b) {
                return b - a;
            }
        };

        template <auto prime, ring_number N>
        struct inverse<times<prime_field<prime, N>>, prime_field<prime, N>> {
            constexpr nonzero<prime_field<prime, N>> operator () (const nonzero<prime_field<prime, N>> &a, const nonzero<prime_field<prime, N>> &b) {
                return b / a;
            }
        };

        template <auto prime, ring_number N>
        struct divide<prime_field<prime, N>, prime_field<prime, N>> {
            constexpr prime_field<prime, N> operator () (const prime_field<prime, N> &a, const nonzero<prime_field<prime, N>> &b) {
                if (b == 0) throw division_by_zero {};
                return a / b;
            }
        };
    }
    
    template <auto P, ring_number N>
    constexpr bool inline operator == (const prime_field<P, N> &a, const prime_field<P, N> &b) {
        return static_cast<const number::modular<P, N> &> (a) == static_cast<const number::modular<P, N> &> (b);
    }
    
    template <auto P, ring_number N>
    constexpr prime_field<P, N> inline prime_field<P, N>::operator + (const prime_field<P, N> &e) const {
        return prime_field<P, N> {
            static_cast<const number::modular<P, N> &> (*this) +
            static_cast<const number::modular<P, N> &> (e)};
    }
    
    template <auto P, ring_number N>
    constexpr prime_field<P, N> inline prime_field<P, N>::operator - (const prime_field &e) const {
        return {static_cast<const number::modular<P, N> &> (*this) - static_cast<const number::modular<P, N> &> (e)};
    }
    
    template <auto P, ring_number N>
    constexpr prime_field<P, N> inline prime_field<P, N>::operator * (const prime_field &e) const {
        return {static_cast<const number::modular<P, N> &> (*this) * static_cast<const number::modular<P, N> &> (e)};
    }
    
    template <auto P, ring_number N>
    constexpr prime_field<P, N> inline prime_field<P, N>::inverse () const {
        if (*this == prime_field {0}) throw division_by_zero {};
        return prime_field {*number::invert (static_cast<const number::modular<P, N> &> (*this))};
    }
    
    template <auto P, ring_number N>
    constexpr prime_field<P, N> inline prime_field<P, N>::operator / (const prime_field &e) const {
        return *this * e.inverse ();
    }
}

#endif


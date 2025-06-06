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
#include <data/tools.hpp>

namespace data::math {
    template <auto P, typename N = decltype (P)> struct prime_field_element;
    
    template <auto P, typename N>
    bool operator == (const prime_field_element<P, N> &, const prime_field_element<P, N> &);
    
    template <auto P, typename N> struct prime_field;
    
    template <auto P, typename N>
    struct prime_field_element : ptr<number::modular<P, N>> {
    
        prime_field_element operator + (const prime_field_element &) const;
        prime_field_element operator - (const prime_field_element &) const;
        prime_field_element operator * (const prime_field_element &) const;
        prime_field_element operator / (const prime_field_element &) const;
        
        prime_field_element inverse () const;
        
        bool valid () const {
            return *this != nullptr && (*this)->valid ();
        }

        template<typename... X>
        prime_field_element (X... x) : ptr<number::modular<P, N>> (std::make_shared<number::modular<P, N>> (x...)) {}
        
    private:
        prime_field_element () : ptr<number::modular<P, N>> {} {}
        
        friend struct prime_field<P, N>;
    };
    
    template <auto P, typename N = decltype (P)>
    struct prime_field {
        number::prime<N> Modulus;
        prime_field (number::prime<N> p) : Modulus {p.Prime == N (P) ? p : number::prime<N> {}} {}
        
        bool valid () const {
            return Modulus.valid () && Modulus.Prime == N (P);
        }
        
        template <typename... X>
        prime_field_element<P, N> make (X... x);
    };

    template <auto P, typename N>
    set<prime_field_element<P, N>> square_root (prime_field_element<P, N>);

    template <auto P, typename N>
    std::ostream inline &operator << (std::ostream &o, const prime_field_element<P, N> &m) {
        return o << "f<" << P << ">{" << m->Value << "}";
    }

    template <auto prime, typename N>
    struct times<prime_field_element<prime, N>> {
        prime_field_element<prime, N> operator () (const prime_field_element<prime, N> &a, const prime_field_element<prime, N> &b) {
            return a * b;
        }

        nonzero<prime_field_element<prime, N>> operator ()
        (const nonzero<prime_field_element<prime, N>> &a, const nonzero<prime_field_element<prime, N>> &b) {
            return nonzero<prime_field_element<prime, N>> {a.Value * b.Value};
        }
    };
    
    template <auto prime, typename N>
    struct identity<plus<prime_field_element<prime, N>>,
        prime_field_element<prime, N>>
        : identity<plus<N>, N> {
        prime_field_element<prime, N> operator () () {
            return {identity<plus<N>, N>::value ()};
        }
    };
    
    template <auto prime, typename N>
    struct identity<times<prime_field_element<prime, N>>,
        prime_field_element<prime, N>>
        : identity<times<N>, N> {
        prime_field_element<prime, N> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <auto prime, typename N>
    struct inverse<plus<prime_field_element<prime, N>>, prime_field_element<prime, N>> {
        prime_field_element<prime, N> operator () (const prime_field_element<prime, N> &a, const prime_field_element<prime, N> &b) {
            return b - a;
        }
    };

    template <auto prime, typename N>
    struct inverse<times<prime_field_element<prime, N>>, prime_field_element<prime, N>> {
        nonzero<prime_field_element<prime, N>> operator ()
        (const nonzero<prime_field_element<prime, N>> &a, const nonzero<prime_field_element<prime, N>> &b) {
            return b / a;
        }
    };

    template <auto prime, typename N>
    struct divide<prime_field_element<prime, N>, prime_field_element<prime, N>> {
        prime_field_element<prime, N> operator ()
        (const prime_field_element<prime, N> &a, const nonzero<prime_field_element<prime, N>> &b) {
            if (b == 0) throw division_by_zero {};
            return a / b;
        }
    };
    
    template <auto P, typename N>
    bool inline operator == (const prime_field_element<P, N> &a, const prime_field_element<P, N> &b) {
        if (static_cast<ptr<number::modular<P, N>>> (a) == static_cast<ptr<number::modular<P, N>>> (b)) return true;
        return *static_cast<ptr<number::modular<P, N>>> (a) == *static_cast<ptr<number::modular<P, N>>> (b);
    }
    
    template <auto P, typename N>
    prime_field_element<P, N> inline
    prime_field_element<P, N>::operator + (
        const prime_field_element<P, N> &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () + *e.get ()};
    }
    
    template <auto P, typename N>
    prime_field_element<P, N> inline
    prime_field_element<P, N>::operator - (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () - *e.get ()};
    }
    
    template <auto P, typename N>
    prime_field_element<P, N> inline
    prime_field_element<P, N>::operator * (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () * *e.get ()};
    }
    
    template <auto P, typename N>
    prime_field_element<P, N>
    prime_field_element<P, N>::inverse () const {
        if (*this == nullptr || *this == prime_field_element {0}) return {};
        return prime_field_element {data::invert_mod<N> (this->get()->Value, N {P})};
    }
    
    template <auto P, typename N>
    inline prime_field_element<P, N>
    prime_field_element<P, N>::operator / (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        if (e == prime_field_element {0}) throw division_by_zero {};
        return *this * e.inverse ();
    }
    
    template <auto P, typename N>
    template <typename... X>
    inline prime_field_element<P, N> prime_field<P, N>::make (X... x) {
        if (!valid ()) return {};
        return prime_field_element<P, N> (x...);
    }
}

#endif


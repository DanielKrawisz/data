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
    template <typename N, auto P> struct prime_field_element;
    
    template <typename N, auto P>
    bool operator == (const prime_field_element<N, P> &, const prime_field_element<N, P> &);
    
    template <typename N, auto P> struct prime_field;
    
    template <typename N, auto P>
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
        
        friend struct prime_field<N, P>;
    };
    
    template <typename N, auto P>
    struct prime_field {
        number::prime<N> Modulus;
        prime_field (number::prime<N> p) : Modulus {p.Prime == N (P) ? p : number::prime<N> {}} {}
        
        bool valid () const {
            return Modulus.valid () && Modulus.Prime == N (P);
        }
        
        template <typename... X>
        prime_field_element<N, P> make (X... x);
    };

    template <typename N, auto P>
    set<prime_field_element<N, P>> square_root (prime_field_element<N, P>);

    template <typename N, auto P>
    std::ostream inline &operator << (std::ostream &o, const prime_field_element<N, P> &m) {
        return o << "f<" << P << ">{" << m->Value << "}";
    }

    template <typename N, auto prime>
    struct times<prime_field_element<N, prime>> {
        prime_field_element<N, prime> operator () (const prime_field_element<N, prime> &a, const prime_field_element<N, prime> &b) {
            return a * b;
        }

        nonzero<prime_field_element<N, prime>> operator ()
        (const nonzero<prime_field_element<N, prime>> &a, const nonzero<prime_field_element<N, prime>> &b) {
            return nonzero<prime_field_element<N, prime>> {a.Value * b.Value};
        }
    };
    
    template <typename N, auto prime>
    struct identity<plus<prime_field_element<N, prime>>,
        prime_field_element<N, prime>>
        : identity<plus<N>, N> {
        prime_field_element<N, prime> operator () () {
            return {identity<plus<N>, N>::value ()};
        }
    };
    
    template <typename N, auto prime>
    struct identity<times<prime_field_element<N, prime>>,
        prime_field_element<N, prime>>
        : identity<times<N>, N> {
        prime_field_element<N, prime> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <typename N, auto prime>
    struct inverse<plus<prime_field_element<N, prime>>, prime_field_element<N, prime>> {
        prime_field_element<N, prime> operator () (const prime_field_element<N, prime> &a, const prime_field_element<N, prime> &b) {
            return b - a;
        }
    };

    template <typename N, auto prime>
    struct inverse<times<prime_field_element<N, prime>>, prime_field_element<N, prime>> {
        nonzero<prime_field_element<N, prime>> operator ()
        (const nonzero<prime_field_element<N, prime>> &a, const nonzero<prime_field_element<N, prime>> &b) {
            return b / a;
        }
    };

    template <typename N, auto prime>
    struct divide<prime_field_element<N, prime>, prime_field_element<N, prime>> {
        prime_field_element<N, prime> operator ()
        (const prime_field_element<N, prime> &a, const nonzero<prime_field_element<N, prime>> &b) {
            if (b == 0) throw division_by_zero {};
            return a / b;
        }
    };
    
    template <typename N, auto P>
    bool inline operator == (const prime_field_element<N, P> &a, const prime_field_element<N, P> &b) {
        if (static_cast<ptr<number::modular<P, N>>> (a) == static_cast<ptr<number::modular<P, N>>> (b)) return true;
        return *static_cast<ptr<number::modular<P, N>>> (a) == *static_cast<ptr<number::modular<P, N>>> (b);
    }
    
    template <typename N, auto P>
    prime_field_element<N, P> inline
    prime_field_element<N, P>::operator + (
        const prime_field_element<N, P> &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () + *e.get ()};
    }
    
    template <typename N, auto P>
    prime_field_element<N, P> inline
    prime_field_element<N, P>::operator - (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () - *e.get ()};
    }
    
    template <typename N, auto P>
    prime_field_element<N, P> inline
    prime_field_element<N, P>::operator * (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () * *e.get ()};
    }
    
    template <typename N, auto P>
    prime_field_element<N, P>
    prime_field_element<N, P>::inverse () const {
        if (*this == nullptr || *this == prime_field_element {0}) return {};
        return prime_field_element {data::invert_mod<N> (this->get()->Value, N {P})};
    }
    
    template <typename N, auto P>
    inline prime_field_element<N, P>
    prime_field_element<N, P>::operator / (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        if (e == prime_field_element {0}) throw division_by_zero {};
        return *this * e.inverse ();
    }
    
    template <typename N, auto P>
    template <typename... X>
    inline prime_field_element<N, P> prime_field<N, P>::make (X... x) {
        if (!valid ()) return {};
        return prime_field_element<N, P> (x...);
    }
}

#endif


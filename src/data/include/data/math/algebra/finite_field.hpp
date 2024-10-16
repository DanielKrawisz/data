// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ALGEBRA_FINITE_FIELD
#define DATA_MATH_ALGEBRA_FINITE_FIELD

#include <data/math/number/prime.hpp>
#include <data/math/number/modular.hpp>
#include <data/math/field.hpp>
#include <data/math/number/extended_euclidian.hpp>
#include <data/math/commutative.hpp>
#include <data/math/associative.hpp>

namespace data::math {
    template <typename N, typename Z, auto P> struct prime_field_element;
    
    template <typename N, typename Z, auto P>
    bool operator == (const prime_field_element<N, Z, P> &, const prime_field_element<N, Z, P> &);
    
    template <typename N, typename Z, auto P> struct prime_field;
    
    template <typename N, typename Z, auto P>
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
        
        friend struct prime_field<N, Z, P>;
    };
    
    template <typename N, typename Z, auto P>
    struct prime_field {
        number::prime<N> Modulus;
        prime_field (number::prime<N> p) : Modulus {p.Prime == N (P) ? p : number::prime<N> {}} {}
        
        bool valid () const {
            return Modulus.valid () && Modulus.Prime == N (P);
        }
        
        template <typename... X>
        prime_field_element<N, Z, P> make (X... x);
    };

    template <typename N, typename Z, auto P>
    std::ostream inline &operator << (std::ostream &o, const prime_field_element<N, Z, P> &m) {
        return o << "f<" << P << ">{" << m->Value << "}";
    }

    template <typename N, typename Z, auto prime>
    struct times<prime_field_element<N, Z, prime>> {
        prime_field_element<N, Z, prime> operator () (const prime_field_element<N, Z, prime> &a, const prime_field_element<N, Z, prime> &b) {
            return a * b;
        }

        nonzero<prime_field_element<N, Z, prime>> operator ()
        (const nonzero<prime_field_element<N, Z, prime>> &a, const nonzero<prime_field_element<N, Z, prime>> &b) {
            return nonzero<prime_field_element<N, Z, prime>> {a.Value * b.Value};
        }
    };
    
    template <typename N, typename Z, auto prime>
    struct commutative<plus<prime_field_element<N, Z, prime>>, 
        prime_field_element<N, Z, prime>>
        : commutative<plus<N>, N> {};
    
    template <typename N, typename Z, auto prime>
    struct associative<plus<prime_field_element<N, Z, prime>>,
        prime_field_element<N, Z, prime>>
        : associative<plus<N>, N> {};
    
    template <typename N, typename Z, auto prime>
    struct commutative<times<prime_field_element<N, Z, prime>>,
        prime_field_element<N, Z, prime>>
        : commutative<times<N>, N> {};
    
    template <typename N, typename Z, auto prime>
    struct associative<times<prime_field_element<N, Z, prime>>,
        prime_field_element<N, Z, prime>>
        : associative<times<N>, N> {};
    
    template <typename N, typename Z, auto prime>
    struct identity<plus<prime_field_element<N, Z, prime>>,
        prime_field_element<N, Z, prime>>
        : identity<plus<N>, N> {
        prime_field_element<N, Z, prime> operator () () {
            return {identity<plus<N>, N>::value ()};
        }
    };
    
    template <typename N, typename Z, auto prime>
    struct identity<times<prime_field_element<N, Z, prime>>,
        prime_field_element<N, Z, prime>>
        : identity<times<N>, N> {
        prime_field_element<N, Z, prime> operator () () {
            return {identity<times<N>, N>::value ()};
        }
    };
    
    template <typename N, typename Z, auto prime>
    struct inverse<plus<prime_field_element<N, Z, prime>>, prime_field_element<N, Z, prime>> {
        prime_field_element<N, Z, prime> operator () (const prime_field_element<N, Z, prime> &a, const prime_field_element<N, Z, prime> &b) {
            return b - a;
        }
    };

    template <typename N, typename Z, auto prime>
    struct inverse<times<prime_field_element<N, Z, prime>>, prime_field_element<N, Z, prime>> {
        nonzero<prime_field_element<N, Z, prime>> operator ()
        (const nonzero<prime_field_element<N, Z, prime>> &a, const nonzero<prime_field_element<N, Z, prime>> &b) {
            return b / a;
        }
    };

    template <typename N, typename Z, auto prime>
    struct divide<prime_field_element<N, Z, prime>, prime_field_element<N, Z, prime>> {
        prime_field_element<N, Z, prime> operator ()
        (const prime_field_element<N, Z, prime> &a, const nonzero<prime_field_element<N, Z, prime>> &b) {
            if (b == 0) throw division_by_zero {};
            return a / b;
        }
    };
    
    template <typename N, typename Z, auto P>
    bool inline operator == (const prime_field_element<N, Z, P> &a, const prime_field_element<N, Z, P> &b) {
        if (static_cast<ptr<number::modular<P, N>>> (a) == static_cast<ptr<number::modular<P, N>>> (b)) return true;
        return *static_cast<ptr<number::modular<P, N>>> (a) == *static_cast<ptr<number::modular<P, N>>> (b);
    }
    
    template <typename N, typename Z, auto P>
    prime_field_element<N, Z, P> inline 
    prime_field_element<N, Z, P>::operator + (
        const prime_field_element<N, Z, P> &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () + *e.get ()};
    }
    
    template <typename N, typename Z, auto P>
    prime_field_element<N, Z, P> inline
    prime_field_element<N, Z, P>::operator - (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () - *e.get ()};
    }
    
    template <typename N, typename Z, auto P>
    prime_field_element<N, Z, P> inline
    prime_field_element<N, Z, P>::operator * (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        return {*this->get () * *e.get ()};
    }
    
    template <typename N, typename Z, auto P>
    prime_field_element<N, Z, P> 
    prime_field_element<N, Z, P>::inverse () const {
        if (*this == nullptr || *this == prime_field_element {0}) return {};
        Z bt = number::euclidian::extended<N, Z>::algorithm (N {P},
            this->get ()->Value).BezoutT;
        if (bt < 0) bt += N {P};
        return prime_field_element {data::abs (bt)};
    }
    
    template <typename N, typename Z, auto P>
    inline prime_field_element<N, Z, P> 
    prime_field_element<N, Z, P>::operator / (const prime_field_element &e) const {
        if (*this == nullptr || e == nullptr) return {};
        if (e == prime_field_element {0}) throw division_by_zero {};
        return *this * e.inverse ();
    }
    
    template <typename N, typename Z, auto P>
    template <typename... X>
    inline prime_field_element<N, Z, P> prime_field<N, Z, P>::make (X... x) {
        if (!valid ()) return {};
        return prime_field_element<N, Z, P> (x...);
    }
}

#endif

